#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <sqlite3.h> 
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>

#define ntohll(x) ( ( (uint64_t)(ntohl( (uint32_t)((x << 32) >> 32) )) << 32) | ntohl( ((uint32_t)(x >> 32)) ) )
#define htonll(x) ntohll(x)

#define MAX_INS 100000000
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


static void inline executeQuery(sqlite3 *db,char *sql,int num_paq)
{
//return;
char *zErrMsg = 0;
int rc;




 /* Execute SQL statement */
                        rc = sqlite3_exec(db, sql, /*callback*/NULL, 0, &zErrMsg);
                        if( rc != SQLITE_OK ){
                                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                                sqlite3_free(zErrMsg);
                        }


}


int main(int argc, char *argv[])
{


//variables medidadas activas
uint32_t capacidad;
uint32_t ancho_banda;
uint32_t OWD;
uint32_t RTT;
uint32_t jitter;
uint32_t packet_loss;
//varibale MRTG
uint32_t cont_bytes;
uint32_t cont_paquetes;
uint64_t timestamp;
uint32_t flujos_con;
//variable flujos
uint32_t srcip;
uint32_t dstip;
uint16_t srcport;
uint16_t dstport;
uint8_t proto;

uint64_t tiempo_inicio;
uint64_t tiempo_final;
uint32_t contador_bytes;
uint32_t contador_paquetes;
char*addr1;
char *proto_trans;
//varibale inet_ntoa
struct in_addr antelope;
struct in_addr antelope1;
char *some_addr;
//variables base de datos
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;
sqlite3_stmt *stmt1;
sqlite3_stmt *stmt2;
sqlite3_stmt *stmt3;
sqlite3_stmt *stmt4;
sqlite3_stmt *stmt5;
sqlite3_stmt *stmt6;

int num_paq=0;
int num_paq_file=0;
int num_paq_pair=0;
int num_paq_train=0;
int num_paq_mrtg=0;
int num_paq_flujos=0;

uint64_t num_bytes=0;
uint64_t num_bytes_file=0;
uint64_t num_bytes_pair=0;
uint64_t num_bytes_train=0;
uint64_t num_bytes_mrtg=0;
uint64_t num_bytes_flujos=0;

uint64_t num_bits=0;
uint64_t num_bits_file=0;
uint64_t num_bits_pair=0;
uint64_t num_bits_train=0;
uint64_t num_bits_mrtg=0;
uint64_t num_bits_flujos=0;

double tiempo_tot_file_transfer=0;
double tiempo_tot_packet_pair=0;
double tiempo_tot_packet_train=0;
double tiempo_tot_mrtg=0;
double tiempo_tot_flujos=0;



//abrimos la base de datos
 rc = sqlite3_open(argv[1], &db);

   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return 1;

   }else{
      //fprintf(stderr, "Opened database successfully\n");
   }


  sqlite3_exec(db, "PRAGMA auto_vacuum=NONE;", NULL, 0, &zErrMsg); 
  sqlite3_exec(db, "PRAGMA locking_mode=EXCLUSIVE;", NULL, 0, &zErrMsg); 
 sqlite3_exec(db, "PRAGMA synchronous=OFF ;", NULL, 0, &zErrMsg); 
 sqlite3_exec(db, "PRAGMA count_changes=OFF; ", NULL, 0, &zErrMsg); 
  sqlite3_exec(db, "PRAGMA temp_store = MEMORY;", NULL, 0, &zErrMsg); 
  sqlite3_exec(db, "PRAGMA journal_mode = OFF;", NULL, 0, &zErrMsg); 
   sqlite3_exec(db, "PRAGMA cache_size = 4000000;", NULL, 0, &zErrMsg); 
   sqlite3_exec(db, "PRAGMA threads = 6; ", NULL, 0, &zErrMsg); 
   /* Create SQL statement */
  sql = sqlite3_mprintf("DELETE FROM entorno_active_measurement_file_transfer"); //VACIAR FILE TRANFER
			
                       
executeQuery(db,sql,num_paq) ;


 /* Create SQL statement */
  sql = sqlite3_mprintf("DELETE FROM entorno_active_measurement_packet_pair"); //VACIAR PACKET PAIR
			
                       
	/* Execute SQL statement */
executeQuery(db,sql,num_paq) ;

 /* Create SQL statement */
  sql = sqlite3_mprintf("DELETE FROM entorno_active_measurement_packet_train"); //VACIAR PACKET TRAIN
			
                       
	/* Execute SQL statement */
executeQuery(db,sql,num_paq) ;

  sql = sqlite3_mprintf("DELETE FROM entorno_flujos"); //VACIAR FLUJOS
			
                       
executeQuery(db,sql,num_paq) ;

 /* Create SQL statement */
  sql = sqlite3_mprintf("DELETE FROM entorno_mrtg"); //VACIAR MRTG
			
                       
	/* Execute SQL statement */

executeQuery(db,sql,num_paq) ;


			

//temporary packet buffers 
struct pcap_pkthdr header; // The header that pcap gives us 
const u_char *packet; // The actual packet 

//open the pcap file 
    pcap_t *handle; 
    char errbuf[PCAP_ERRBUF_SIZE]; 
    char ruta_fichero[]="nmproto.pcap"; 
   
    handle = pcap_open_offline(argv[2], errbuf);   //call pcap library function 
    

    if (handle == NULL) { 
      fprintf(stderr,"Couldn't open pcap file %s: %s\n",argv[2], errbuf); 
      return(2); 
    } 

//obtenemos el tiempo al principio del programa

struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.00);




sqlite3_prepare_v2(db, "insert into entorno_active_measurement_file_transfer (capacity,available_bandwidth,one_way_delay,round_trip_time,jitter,packet_loss) values (?1, ?2,?3,?4,?5,?6);", -1, &stmt1, NULL);      

sqlite3_prepare_v2(db, "insert into entorno_active_measurement_packet_pair (capacity,available_bandwidth,one_way_delay,round_trip_time,jitter,packet_loss) values (?1, ?2,?3,?4,?5,?6);", -1, &stmt2, NULL); 

sqlite3_prepare_v2(db, "insert into entorno_active_measurement_packet_train (capacity,available_bandwidth,one_way_delay,round_trip_time,jitter,packet_loss) values (?1, ?2,?3,?4,?5,?6);", -1, &stmt3, NULL);

sqlite3_prepare_v2(db, "insert into entorno_mrtg (contador_bytes,contador_paquetes,flujos_concurrentes,timestamp) values (?1, ?2,?3,?4);", -1, &stmt4, NULL);


	sqlite3_prepare_v2(db, "insert into entorno_flujos (ip_origen,ip_destino,puerto_origen,puerto_destino,protocolo_transporte,numero_bytes,numero_paquetes,tiempo_inicio,tiempo_fin) values (?1, ?2,?3,?4,?5,?6,?7,?8,?9);", -1, &stmt5, NULL);       /* 1 */

   sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    while ((packet = pcap_next(handle,&header))!=NULL) { 

/*			 if((num_paq%MAX_INS==0)&&(num_paq>0))


                        {
                                 sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL);

                                sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);


                        }*/

        num_paq=1+num_paq;
	num_bytes=num_bytes+header.len;

//	printf("Numero de bytes(hex):%" PRIu64 "  num_paq=%d\n",num_bytes,num_paq);
    // header contains information about the packet (e.g. timestamp) 
       u_char *pkt_ptr = (u_char *)packet; //cast a pointer to the packet data 
        
    //MEDIDAS ACTIVAS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 if(pkt_ptr[46]==33){ //MEDIDA ACTIVA

		 uint64_t division_file=((header.len-50)/33);//tamaño de los datos 33 bytes
		 //printf("Numero de bytes:%" PRIu64 " \n",division_file);
				
		if(pkt_ptr[58]==00){//FILE TRANSFER
                       
			num_paq_file=1+num_paq_file;
			num_bytes_file=num_bytes_file+header.len;
			gettimeofday(&tim, NULL);
			double t1_file_transfer=tim.tv_sec+(tim.tv_usec/1000000.00);
			int x=0;

			while(division_file >0){

				memcpy(&capacidad,&pkt_ptr[59+x], sizeof(uint32_t));
				capacidad=ntohl(capacidad);
				memcpy(&ancho_banda,&pkt_ptr[63+x], sizeof(uint32_t));
				ancho_banda=ntohl(ancho_banda);
			
				memcpy(&OWD,&pkt_ptr[67+x], sizeof(uint32_t));
				OWD=ntohl(OWD);
				memcpy(&RTT,&pkt_ptr[71+x], sizeof(uint32_t));
				RTT=ntohl(RTT);
				memcpy(&jitter,&pkt_ptr[75+x], sizeof(uint32_t));
				jitter=ntohl(jitter);
				memcpy(&packet_loss,&pkt_ptr[79+x], sizeof(uint32_t));
				packet_loss=ntohl(packet_loss);
			

			
                       
				sqlite3_bind_double(stmt1, 1, capacidad);                                        
				sqlite3_bind_double(stmt1, 2, ancho_banda); 
				sqlite3_bind_double(stmt1, 3, OWD);  
				sqlite3_bind_double(stmt1, 4, RTT); 
				sqlite3_bind_double(stmt1, 5, jitter);  
				sqlite3_bind_int(stmt1, 6, packet_loss);

				rc = sqlite3_step(stmt1); 
 				// sqlite3_clear_bindings(stmt1);
				sqlite3_reset(stmt1);
			 


					gettimeofday(&tim, NULL);
			double t2_file_transfer=tim.tv_sec+(tim.tv_usec/1000000.00);
			
			tiempo_tot_file_transfer=tiempo_tot_file_transfer + (t2_file_transfer-t1_file_transfer);			
			
				x=x+33;
				division_file=division_file-1;
			
			}

		}


		else if(pkt_ptr[58]==1){//PACKET PAIR
			num_paq_pair=1+num_paq_pair;
			num_bytes_pair=num_bytes_pair+header.len;
			gettimeofday(&tim, NULL);
			double t1_packet_pair=tim.tv_sec+(tim.tv_usec/1000000.00);
			int x=0;
			while(division_file >0){
			memcpy(&capacidad,&pkt_ptr[59+x], sizeof(uint32_t));
			capacidad=ntohl(capacidad);
			memcpy(&ancho_banda,&pkt_ptr[63+x], sizeof(uint32_t));
			ancho_banda=ntohl(ancho_banda);
			memcpy(&OWD,&pkt_ptr[67+x], sizeof(uint32_t));
			OWD=ntohl(OWD);
			memcpy(&RTT,&pkt_ptr[71+x], sizeof(uint32_t));
			RTT=ntohl(RTT);
			memcpy(&jitter,&pkt_ptr[75+x], sizeof(uint32_t));
			jitter=ntohl(jitter);
			memcpy(&packet_loss,&pkt_ptr[79+x], sizeof(uint32_t));
			packet_loss=ntohl(packet_loss);
			

			
			sqlite3_bind_double(stmt2, 1, capacidad);                                        /* 2 */
			sqlite3_bind_double(stmt2, 2, ancho_banda); 
			sqlite3_bind_double(stmt2, 3, OWD);  
			sqlite3_bind_double(stmt2, 4, RTT); 
			sqlite3_bind_double(stmt2, 5, jitter);  
			sqlite3_bind_int(stmt2, 6, packet_loss);

			rc = sqlite3_step(stmt2); 
			//  sqlite3_clear_bindings(stmt2);
			sqlite3_reset(stmt2);

			gettimeofday(&tim, NULL);
			double t2_packet_pair=tim.tv_sec+(tim.tv_usec/1000000.00);
			
			tiempo_tot_packet_pair=tiempo_tot_packet_pair + (t2_packet_pair-t1_packet_pair);

			x=x+33;
			division_file=division_file-1;	

		}
		}
		else if(pkt_ptr[58]==2){//PACKET TRAIN
			num_paq_train=1+num_paq_train;
			num_bytes_train=num_bytes_train+header.len;
			gettimeofday(&tim, NULL);
			double t1_packet_train=tim.tv_sec+(tim.tv_usec/1000000.00);
			
			int x=0;
			while(division_file >0){
			memcpy(&capacidad,&pkt_ptr[59+x], sizeof(uint32_t));
			capacidad=ntohl(capacidad);
			memcpy(&ancho_banda,&pkt_ptr[63+x], sizeof(uint32_t));
			ancho_banda=ntohl(ancho_banda);
			memcpy(&OWD,&pkt_ptr[67+x], sizeof(uint32_t));
			OWD=ntohl(OWD);
			memcpy(&RTT,&pkt_ptr[71+x], sizeof(uint32_t));
			RTT=ntohl(RTT);
			memcpy(&jitter,&pkt_ptr[75+x], sizeof(uint32_t));
			jitter=ntohl(jitter);
			memcpy(&packet_loss,&pkt_ptr[79+x], sizeof(uint32_t));
			packet_loss=ntohl(packet_loss);

                         //sql = sqlite3_mprintf("INSERT INTO entorno_active_measurement_packet_train (capacity,available_bandwidth,one_way_delay,round_trip_time,jitter,packet_loss) VALUES ('%d', '%d', '%d', '%d', '%d', '%d');",capacidad,ancho_banda,OWD,RTT,jitter,packet_loss); 
			

			//executeQuery(db,sql,num_paq) ;


			sqlite3_bind_double(stmt3, 1, capacidad);                                        /* 2 */
			sqlite3_bind_double(stmt3, 2, ancho_banda); 
			sqlite3_bind_double(stmt3, 3, OWD);  
			sqlite3_bind_double(stmt3, 4, RTT); 
			sqlite3_bind_double(stmt3, 5, jitter);  
			sqlite3_bind_int(stmt3, 6, packet_loss);

			rc = sqlite3_step(stmt3); 
			//  sqlite3_clear_bindings(stmt3);
			sqlite3_reset(stmt3);

			gettimeofday(&tim, NULL);
			double t2_packet_train=tim.tv_sec+(tim.tv_usec/1000000.00);
			
			tiempo_tot_packet_train=tiempo_tot_packet_train + (t2_packet_train-t1_packet_train);	

		

			x=x+33;
			division_file=division_file-1;		
		}
		}
	}
       

       	else if(pkt_ptr[46]==26){ //MRTG
	num_paq_mrtg=1+num_paq_mrtg;
	num_bytes_mrtg=num_bytes_mrtg+header.len;

	 uint64_t division_mrtg=((header.len-50)/20);//tamaño de los datos 33 bytes
	//printf("Numero de bytes:%" PRIu64 " \n",division_mrtg);		
	gettimeofday(&tim, NULL);
	double t1_mrtg=tim.tv_sec+(tim.tv_usec/1000000.00);
	
	int x=0;
	while(division_mrtg >0){

	memcpy(&cont_bytes,&pkt_ptr[50+x], sizeof(uint32_t));
	cont_bytes=ntohl(cont_bytes);
	memcpy(&cont_paquetes,&pkt_ptr[54+x], sizeof(uint32_t));
	cont_paquetes=ntohl(cont_paquetes);
	//memcpy(&time1,&pkt_ptr[58+x], sizeof(uint32_t));
	//time1=ntohl(time1);
	//memcpy(&time2,&pkt_ptr[62+x], sizeof(uint32_t));
	//time2=ntohl(time2);
	
	memcpy(&timestamp,&pkt_ptr[58+x], sizeof(uint64_t));
	timestamp=be64toh(timestamp);
	double timestamp_dec=timestamp;
	
	timestamp_dec=timestamp_dec/1000000000000000;
	//printf("%d.%d  %d\n",time1,time2,num_paq);
	memcpy(&flujos_con,&pkt_ptr[66+x], sizeof(uint32_t));
	flujos_con=ntohl(flujos_con);
		
		
		
	

			sqlite3_bind_double(stmt4, 1, cont_bytes);                                        /* 2 */
			sqlite3_bind_int(stmt4, 2, cont_paquetes);
			sqlite3_bind_int(stmt4, 3, flujos_con);
			sqlite3_bind_double(stmt4, 4, timestamp_dec);
			
			
			

			rc = sqlite3_step(stmt4); 
			//  sqlite3_clear_bindings(stmt4);
		sqlite3_reset(stmt4);

	gettimeofday(&tim, NULL);
	double t2_mrtg=tim.tv_sec+(tim.tv_usec/1000000.00);
			
	tiempo_tot_mrtg=tiempo_tot_mrtg + (t2_mrtg-t1_mrtg);	
	

			x=x+20;
			division_mrtg=division_mrtg-1;			
       	}
	}
	
      	else if(pkt_ptr[46]==32){ //FLUJO
	//printf("FLUJO\n");
	 uint64_t division_flujos=((header.len-50)/37);//tamaño de los datos 33 bytes
	//printf("Numero de bytes:%" PRIu64 " \n",division_flujos);		
	
	num_paq_flujos=1+num_paq_flujos;
	num_bytes_flujos=num_bytes_flujos+header.len;

	gettimeofday(&tim, NULL);
	double t1_flujo=tim.tv_sec+(tim.tv_usec/1000000.00);
	
	int x=0;
	while(division_flujos >0){
	memcpy(&antelope.s_addr,&pkt_ptr[50+x], sizeof(uint32_t));
	char *ipsrc=inet_ntoa(antelope);
	char ipsrc1[200];
	strcpy(ipsrc1,ipsrc);
	
	memcpy(&antelope1.s_addr,&pkt_ptr[54+x], sizeof(uint32_t));
	char *ipdst=inet_ntoa(antelope1);
	char ipdst1[200];
	strcpy(ipdst1,ipdst);

	//printf("%d %s %s %x %x %p %p\n",num,ipdst1,ipsrc1,antelope.s_addr,antelope1.s_addr,ipdst,ipsrc); 
	memcpy(&srcport,&pkt_ptr[58+x], sizeof(uint16_t));
	srcport=ntohs(srcport);
	memcpy(&dstport,&pkt_ptr[60+x], sizeof(uint16_t));
	dstport=ntohs(dstport);
	memcpy(&proto,&pkt_ptr[62+x], sizeof(uint8_t));
				
	if(proto==6){
		proto_trans= "TCP";
	}
	if(proto==17){
		proto_trans= "UDP";
	}
		
	memcpy(&tiempo_inicio,&pkt_ptr[63+x], sizeof(uint64_t));
	tiempo_inicio=be64toh(tiempo_inicio);
	double tiempo_inicio_dec=tiempo_inicio;
	
	tiempo_inicio_dec=tiempo_inicio_dec/1000000000000000;
		
	memcpy(&tiempo_final,&pkt_ptr[71+x], sizeof(uint64_t));
	tiempo_final=be64toh(tiempo_final);
	double tiempo_final_dec=tiempo_final;
	
	tiempo_final_dec=tiempo_final_dec/1000000000000000;
			
	memcpy(&contador_bytes,&pkt_ptr[79+x], sizeof(uint32_t));
	contador_bytes=ntohl(contador_bytes);
        //printf("%d %d\n",num,contador_bytes);
	memcpy(&contador_paquetes,&pkt_ptr[83+x], sizeof(uint32_t));
	contador_paquetes=ntohl(contador_paquetes);
        //printf("%d %d %d\n",num,contador_bytes,contador_paquetes);
		

	sqlite3_bind_text(stmt5, 1, ipsrc1, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt5, 2,ipdst1, -1, SQLITE_STATIC);  
	sqlite3_bind_int(stmt5, 3, srcport); 
	sqlite3_bind_int(stmt5, 4, dstport);
	sqlite3_bind_text(stmt5, 5,proto_trans, -1, SQLITE_STATIC);  
	sqlite3_bind_double(stmt5, 6, contador_bytes);
	sqlite3_bind_int(stmt5, 7, contador_paquetes);
	sqlite3_bind_double(stmt5, 8, tiempo_inicio_dec);
	sqlite3_bind_double(stmt5, 9, tiempo_final_dec);

	rc = sqlite3_step(stmt5); 
	//  sqlite3_clear_bindings(stmt5);
	sqlite3_reset(stmt5);

	gettimeofday(&tim, NULL);
	double t2_flujo=tim.tv_sec+(tim.tv_usec/1000000.00);
			
	tiempo_tot_flujos=tiempo_tot_flujos + (t2_flujo-t1_flujo);	
		
	
	x=x+37;
	division_flujos=division_flujos-1;		
       }
       }

    }//end internal loop for reading packets (all in one file) 
 


      sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL);
      pcap_close(handle);  //close the pcap file 

	
	sqlite3_finalize(stmt1); 
	sqlite3_finalize(stmt2);
	sqlite3_finalize(stmt3);
	sqlite3_finalize(stmt4);
	sqlite3_finalize(stmt5); 
	
    	 sqlite3_close(db);

	 gettimeofday(&tim, NULL);
         double t2=tim.tv_sec+(tim.tv_usec/1000000.00);
	 printf("Numero de paquetes:%d\n",num_paq);
	 printf("Numero de bytes:%" PRIu64 "\n",num_bytes);
	 num_bits=num_bytes*8;
	 num_bits_file=num_bytes_file*8;
	 num_bits_pair=num_bytes_pair*8;
	 num_bits_train=num_bytes_train*8;
	 num_bits_mrtg=num_bytes_mrtg*8;
	 num_bits_flujos=num_bytes_flujos*8;

	 //printf("Numero de bits:%" PRIu64 "\n",num_bits);
         //printf("tiempo de ejecucion: %.6lf segundos\n", t2-t1);
	 printf("Numero de paquetes analizados por segundo: %.6lf\n", num_paq/(t2-t1));
	 printf("Numero de bits analizados por segundo: %.6lf\n\n", num_bits/(t2-t1));
	 //printf("tiempo file transfer: %.6lf segundosnumero paquetes: %d\n", tiempo_tot_file_transfer,num_paq_file);
	 //printf("numero de bits file transfer:%" PRIu64 "\n",num_bits_file);
	 //printf("numero de bits packet pair:%" PRIu64 "\n",num_bits_pair);
	 //printf("numero de bits packet train:%" PRIu64 "\n",num_bits_train);
	 //printf("numero de bits MRTG:%" PRIu64 "\n",num_bits_mrtg);
	 //printf("numero de bits flujos:%" PRIu64 "\n",num_bits_flujos);
	 //printf("tiempo packet pair: %.6lf segundos numero paquetes: %d\n", tiempo_tot_packet_pair,num_paq_pair);
	 //printf("tiempo packet train: %.6lf segundos numero paquetes: %d\n", tiempo_tot_packet_train,num_paq_train);
	 //printf("tiempo MRTG: %.6lf segundos numero paquetes: %d\n", tiempo_tot_mrtg,num_paq_mrtg);
	 //printf("tiempo flujos: %.6lf segundos numero paquetes: %d\n", tiempo_tot_flujos,num_paq_flujos);
	 printf("Numero de paquetes analizados por segundo file transfer: %.6lf\n", num_paq_file/tiempo_tot_file_transfer);
	 printf("Numero de bits analizados por segundo file transfer: %.6lf\n\n", num_bits_file/tiempo_tot_file_transfer);

	 printf("Numero de paquetes analizados por segundo packet pair: %.6lf\n", num_paq_pair/tiempo_tot_packet_pair);
	 printf("Numero de bits analizados por segundo packet pair: %.6lf\n\n", num_bits_pair/tiempo_tot_packet_pair);

	 printf("Numero de paquetes analizados por segundo packet train: %.6lf\n", num_paq_train/tiempo_tot_packet_train);
	 printf("Numero de bits analizados por segundo packet train: %.6lf\n\n", num_bits_train/tiempo_tot_packet_train);

         printf("Numero de paquetes analizados por segundo MRTG: %.6lf\n", num_paq_mrtg/tiempo_tot_mrtg);
	 printf("Numero de bits analizados por segundo MRTG: %.6lf\n\n", num_bits_mrtg/tiempo_tot_mrtg);

 	 printf("Numero de paquetes analizados por segundo flujos: %.6lf\n", num_paq_flujos/tiempo_tot_flujos);
	 printf("Numero de bits analizados por segundo flujos: %.6lf\n", num_bits_flujos/tiempo_tot_flujos);


		
	
	
}
	

