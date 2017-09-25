

/*
 * netconf-server
 * Author Radek Krejci <rkrejci@cesnet.cz>
 *
 * Example implementation of event-driven NETCONF server using libnetconf.
 *
 * Copyright (C) 2012 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * ALTERNATIVELY, provided that this notice is retained in full, this
 * product may be distributed under the terms of the GNU General Public
 * License (GPL) version 2 or later, in which case the provisions
 * of the GPL apply INSTEAD OF those given above.
 *
 * This software is provided ``as is, and any express or implied
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the company or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <event2/event.h>
 #include <sys/sysinfo.h>
   #include <sys/utsname.h>

#include <libnetconf.h>
#include <string.h>
#define VERSION "0.1"

struct srv_config {
	struct nc_session *session;
	ncds_id* dsids;
	struct event_base *event_base;
	struct event *event_input;
};

int clb_print(const char* msg)
{
	syslog(LOG_CRIT,"%s", msg);
	return (EXIT_SUCCESS);
}


void print_version()
{
	fprintf(stdout, "libnetconf server version: %s\n", VERSION);
	fprintf(stdout, "compile time: %s, %s\n", __DATE__, __TIME__);
}

void process_rpc(evutil_socket_t in, short events, void *arg)
{
	nc_rpc *rpc = NULL;
	nc_reply *reply = NULL;
	NC_RPC_TYPE req_type;
	NC_OP req_op;
	struct srv_config *config = (struct srv_config*)arg;
	int timeout=0;

	/* receive incoming message */
	if (nc_session_recv_rpc(config->session, timeout,&rpc) == 0) {//OJO TIMEOUT
		/* something really bad happend, and communication os not possible anymore */
		nc_session_free(config->session);
		event_base_loopbreak(config->event_base);
		return;
	}

	/* process it */
	char *aa=nc_rpc_dump 	(rpc); 	
	syslog(LOG_CRIT,"rpc %s", aa);
	req_type = nc_rpc_get_type(rpc);
	req_op = nc_rpc_get_op(rpc);
	syslog(LOG_CRIT,"Type: %d", req_type);
	 if (req_type == NC_RPC_SESSION) {
		/* process operations affectinf session */

	
		if (req_op == NC_OP_CLOSESESSION) {
			clb_print("NC_OP_CLOSESESSION");	
			/* exit the event loop immediately without processing any following request */
			event_base_loopbreak(config->event_base);
		} else if (req_op == NC_OP_KILLSESSION) {
			/* todo: kill the requested session */
			clb_print("NC_OP_KILLSESSION");	
			reply = nc_reply_error(nc_err_new(NC_ERR_OP_NOT_SUPPORTED));
		}
	syslog(LOG_CRIT,"FIN");
	} else if (req_type == NC_RPC_DATASTORE_READ) 
	{
		/* process operations reading datastore */
		/*nc_reply_data: create rpc-reply response with <data> content*/
				syslog(LOG_CRIT,"OP %d", req_op);	
		switch (req_op) {

			case NC_OP_GET:
			{

					char *aa=nc_rpc_dump 	(rpc); 	
					syslog(LOG_CRIT,"AAAAAAAAAAAAAAAAAAA %s", aa);
					aa=nc_rpc_get_op_content 	(rpc) 	;
					syslog(LOG_CRIT,"BBBBBBBBBBBB %s", aa);
					clb_print("NC_OP_GET");	
		//			clb_print("Antes %d",ids[0]);	
					clb_print("Antes");	
					reply=ncds_apply_rpc2all(config->session,rpc,&config->dsids);
					clb_print("Despues");	
					if(reply==NULL)
					{
						clb_print("NULL ERROR");
						reply = nc_reply_error(nc_err_new(NC_ERR_OP_NOT_SUPPORTED));
					}
					else
					{
						clb_print("OKKK");

					}
				
			}break;
			case NC_OP_GETCONFIG:
			{
					clb_print("NC_OP_GETCONFIG");	
					reply = nc_reply_data("<libnetconf-server xmlns=\"urn:cesnet:tmc:libnetconf-server:0.1\"/>");
			}break;
			case NC_OP_GETSCHEMA:
			{
					clb_print("NC_OP_GETSCHEMA");	
					reply = ncds_apply_rpc2all(config->session, rpc, &config->dsids);

			}break;
			default:
			{
					clb_print("DEFAULT");	
					reply = nc_reply_error(nc_err_new(NC_ERR_OP_NOT_SUPPORTED));
			}break;
		}
		
	} 
	else
	{

		clb_print("OTHERS");	
		reply = ncds_apply_rpc2all(config->session, rpc,NULL);
	 	syslog(LOG_CRIT,"replyA %s", nc_reply_get_data(reply));
	}
	/* create reply */
	if (reply == NULL) {
		/*
		 * Following operations are handled by this answer:
		 * - edit-config
		 * - copy-config
		 * - delete-config
		 * - lock, unlock
		 * Mentioned operations will be handled correctly in the future,
		 * for now, server does not work with any real datastore.
		 */
		clb_print("REP NULL");
		reply = nc_reply_ok();
	}
	clb_print("REP OK");
	/* and send the reply to the client */

	nc_session_send_reply(config->session, rpc, reply);
	nc_rpc_free(rpc);
	nc_reply_free(reply);

	/* and run again when a next message comes */
}

int
get_clock_current_datetime (char* current_datetime,struct nc_err** error)
{
	time_t current_time;
	current_time = time(NULL);
	const struct tm* timeptr;
	char tmp [255]="";
	
	if (current_time<0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to retrieve current system time.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	timeptr = localtime(& current_time);
	
	if (strftime(current_datetime,80,"%Y-%m-%dT:%H:%M:%S",timeptr) == 0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to convert current system time to desired format.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	if (strftime(tmp,10,"Z%z",timeptr) == 0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to convert current system time to desired format.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	
	tmp[6]=0;
	tmp[5]=tmp[4];
	tmp[4]=tmp[3];
	tmp[3]=':';
	
	strcat (current_datetime,tmp);
	
	return 0;
}



int
get_clock_boot_datetime (char* clock_boot_datetime, struct nc_err** error)
{
	struct sysinfo info;
	time_t current_time;
	time_t boot_datetime;
	const struct tm* timeptr;
	char tmp [255]="";
	
	if (sysinfo(&info)<0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"State info error: Unable to retrieve system boot time.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	current_time = time(NULL);
	if (current_time<0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to retrieve current system time to compute boot time.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	boot_datetime=current_time-info.uptime;
	
	timeptr = localtime(&boot_datetime);
	if (strftime(clock_boot_datetime,80,"%Y-%m-%dT:%H:%M:%S",timeptr) == 0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to convert system boot time to desired format.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	if (strftime(tmp,10,"%z",timeptr) == 0)
	{
		(*error)=nc_err_new(NC_ERR_OP_FAILED);
		nc_err_set((*error),NC_ERR_PARAM_MSG,"System state info error: Unable to convert system boot time to desired format.");
		nc_err_set((*error),NC_ERR_PARAM_TYPE,"application");
		return -1;
	}
	tmp[6]=0;
	tmp[5]=tmp[4];
	tmp[4]=tmp[3];
	tmp[3]=':';
	
	strcat (clock_boot_datetime,tmp);
	
	return 0;
}
char* perform_ping  (const char *model, const char *running, struct nc_err **e)
{

	

	syslog(LOG_CRIT,"Running %s", running);
	syslog(LOG_CRIT,"Model %s", model);

	char* state_info_string = malloc (1500*sizeof(char));
	
	
	

	sprintf (state_info_string,"<measurements xmlns=\"urn:ietf:params:xml:ns:yang:custom-measurements\">\n\t<ping>AAAAAA</ping></measurements>\n\n");


	return state_info_string;
}


char*
get_system_state_info (const char *model, const char *running, struct nc_err **e)
{
	struct utsname sys_info;
	char current_datetime[255]="";
	char boot_datetime[255]="";

//	syslog(LOG_CRIT,"Model %s", model);
	uname(&sys_info);
	char* state_info_string = malloc (1500*sizeof(char));

	if (get_clock_current_datetime (current_datetime, e) < 0) return NULL;

	if (get_clock_boot_datetime (boot_datetime, e) < 0) return NULL;

	

	sprintf (state_info_string,"<system xmlns=\"urn:ietf:params:xml:ns:yang:ietf-system\">\n\t<platform>\n\t\t<os-name>%s</os-name>\n\t\t<os-release>%s</os-release>\n\t\t<os-version>%s</os-version>\n\t\t<machine>%s</machine>\n\t\t<nodename>%s</nodename>\n\t</platform>\n\t<clock>\n\t\t<current-datetime>%s</current-datetime>\n\t\t<boot-datetime>%s</boot-datetime>\n\t</clock>\n</system>\n\n ",sys_info.sysname,sys_info.release,sys_info.version,sys_info.machine,sys_info.nodename,current_datetime,boot_datetime);


return state_info_string;
}

int main(int argc, char *argv[])
{
	struct srv_config config;
	struct ncds_ds* datastore;
	

	/* set verbosity and function to print libnetconf's messages */
	nc_verbosity(NC_VERB_DEBUG);
	
	/* set message printing into the system log */
	openlog("ncserver", LOG_PID, LOG_DAEMON);
	//nc_callback_print(clb_print);
	clb_print("Inicializando");

	/* init libnetconf (at least) with Notification subsystem */
	if(nc_init(NC_INIT_CLIENT| NC_INIT_SINGLELAYER| NC_INIT_NACM)<0)
	//if(nc_init(NC_INIT_NOTIF | NC_INIT_MONITORING| NC_INIT_WD | NC_INIT_SINGLELAYER) < 0)
	{
		nc_verb_error("Library initialization failed");
		printf("AAAAAAAAAAAAAAAAAA\n");
	}


	/* prepare configuration datastore */
	/*datastore = ncds_new(NCDS_TYPE_FILE, "/home/tito/netconf/models/ietf-system.yin", get_system_state_info);//OJO NULL
	if (datastore == NULL) {
		clb_print("Hola");
		clb_print("Datastore preparing failed.");
		
		return (EXIT_FAILURE);
	}
	if (ncds_file_set_path(datastore, "/home/tito/netconf/models/datastore.xml") != 0) {
		clb_print("Linking datastore to a file failed.");
		return (EXIT_FAILURE);
	}*/

	//config.dsids=malloc(2*sizeof(ncds_id));
	//config.dsids[0] = ncds_init(datastore);
	config.dsids=malloc(1*sizeof(ncds_id));
	/*if (config.dsids[0] <= 0) {
		clb_print("Initiating datastore failed.");
		return (EXIT_FAILURE);
	}
*/

	datastore = ncds_new_transapi(NCDS_TYPE_FILE, "/home/tfm/mininet/netconf_05052017/models/models3/toaster.yin", "/home/tfm/mininet/netconf_05052017/libs/toaster.so");//OJO NULL

	if (datastore == NULL) {
		clb_print("Hola");
		clb_print("Datastore preparing failed2.");
		
		return (EXIT_FAILURE);
	}
	if (ncds_file_set_path(datastore, "/home/tfm/mininet/netconf_05052017/models/datastore2.xml") != 0) {
		clb_print("Linking datastore to a file failed.");
		return (EXIT_FAILURE);
	}

	
	config.dsids[0] = ncds_init(datastore);
	if (config.dsids[0] <= 0) {
		clb_print("Initiating datastore failed.");
		return (EXIT_FAILURE);
	}


		/* create the NETCONF session */

	
	ncds_consolidate();
	ncds_feature_enable("ietf-system.yin","authentication");
	ncds_feature_enable("ietf-system.yin","local-users");
	ncds_feature_enable("ietf-system.yin","crypt-hash-md5 ");
	ncds_feature_enable("ietf-system.yin","crypt-hash-sha-256");
	ncds_feature_enable("ietf-system.yin","crypt-hash-sha-512");
	ncds_feature_enable("ietf-system.yin","ntp");
	ncds_feature_enable("ietf-system.yin","timezone-location");
	
	config.session = nc_session_accept(NULL);
	
	if (config.session == NULL) {
	
		clb_print("Session not established.\n");
		return (EXIT_FAILURE);
	}
		

	
	/* prepare event base (libevent) */
	config.event_base = event_base_new();
	if (config.event_base == NULL) {
		clb_print("Event base initialisation failed.\n");
		return (EXIT_FAILURE);
	}

	/* create the event of receiving incoming message from the NETCONF client */
	config.event_input = event_new(config.event_base, (evutil_socket_t)nc_session_get_eventfd(config.session), EV_READ | EV_PERSIST, process_rpc, (void*) (&config));

	/* add the event to the event base and run the main event loop */
	event_add (config.event_input, NULL);
	event_base_dispatch(config.event_base);

	/* cleanup */
	event_free(config.event_input);
	event_base_free(config.event_base);
	if (nc_session_get_status(config.session) == NC_SESSION_STATUS_WORKING) {
		nc_close();
	}
	nc_session_free(config.session);

	/* bye, bye */
	return (EXIT_SUCCESS);
}
