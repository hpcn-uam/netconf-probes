from django.shortcuts import render
from django.template import RequestContext, loader
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from django.shortcuts import render
from django.template import RequestContext, loader
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from entorno.models import Usuario,Flujos,MRTG,Active_Measurement_file_transfer,Active_Measurement_packet_pair,Active_Measurement_packet_train,Iperf,Ping
import sys
import sqlite3
import os
from subprocess import call
from subprocess import check_output
import shlex
from subprocess import Popen,PIPE
import sqlite3
import time
import datetime

globvar=1

def index(request):
    #template = loader.get_template('entorno/prueb.html')
    return render(request,'entorno/index.html',{})


def formini(request):
        usu=request.POST['username']
        con=request.POST['password']
        try:
		autor=Usuario.objects.get(usuario=usu)
		try:
			password=Usuario.objects.get(contrasena=con)
			return HttpResponseRedirect(reverse('entorno:inicio'))	
			
		except Usuario.DoesNotExist:
			return HttpResponse("Error password")

	except Usuario.DoesNotExist: 
		return HttpResponse("Error usuario")

def formreg(request):
        usu=request.POST['usernamesignup']
        email=request.POST['emailsignup']
        con=request.POST['passwordsignup']
        confirm=request.POST['passwordsignup_confirm']
	       
        if con==confirm:

		try:
		        #global globvar    # Needed to modify global copy of globvar
    			#globvar = globvar+1						
			conn = sqlite3.connect('/home/tito/tfg/db.sqlite3')
			c = conn.cursor()
			
			# Insert a row of data
			#c.execute("INSERT INTO entorno_usuario VALUES ('4',usu,email,con)")
			c.execute("INSERT INTO entorno_usuario VALUES (?, ?, ?, ?);", ( globvar,usu, email, con))

			# Save (commit) the changes
			conn.commit()

			# We can also close the connection if we are done with it.
			# Just be sure any changes have been committed or they will be lost.
			conn.close()
			return HttpResponseRedirect(reverse('entorno:index'))
	
		except sqlite3.Error, e:
    
   			 print "Error %s:" % e.args[0]
    			 sys.exit(1)	
		#except:
			#return HttpResponse("No se puede acceder a la base de datos")

	else: 
		return HttpResponse("Error al introducir la contrasena")

def formflujos(request):
	p=Flujos.objects.all()
	x=Flujos.objects.all()
        ip_origen_post=request.POST['ip_origen']
	ip_destino_post=request.POST['ip_destino']
 	puerto_origen_post=request.POST['puerto_origen']
 	puerto_destino_post=request.POST['puerto_destino']
	print(ip_origen_post,ip_destino_post,puerto_origen_post,puerto_destino_post)
        select=request.POST['seleccion']
    
	if select=="1":
		w=1
        else:
		w=2
	
        #if not ip_origen_post and not ip_destino_post and not puerto_origen_post and not puerto_destino_post:
			#c=Flujos.objects.all()

        #else   ip_origen_post or not ip_destino_post or not puerto_origen_post or not puerto_destino_post:
			
			#c=Flujos.objects.filter(ip_origen=ip_origen_post,ip_destino=ip_destino_post,puerto_origen=puerto_origen_post,puerto_destino=puerto_destino_post)
		
	if ip_origen_post:
		p=p.filter(ip_origen=ip_origen_post)
	if ip_destino_post:
		p=p.filter(ip_destino=ip_destino_post)
	if puerto_origen_post:
		p=p.filter(puerto_origen=puerto_origen_post)
	if puerto_destino_post:
		p=p.filter(puerto_destino=puerto_destino_post)
	
        t=loader.get_template('entorno/flujos.html')
	x=RequestContext(request,{'x':x,'p':p,'w':w})
	return HttpResponse(t.render(x))
         	
def formflujos_fprobe(request):
	s=Flujos.objects.all()
	p=Flujos.objects.all()
	x=Flujos.objects.all()
        ip_origen_post=request.POST['ip_origen']
	ip_destino_post=request.POST['ip_destino']
 	puerto_origen_post=request.POST['puerto_origen']
 	puerto_destino_post=request.POST['puerto_destino']
	print(ip_origen_post,ip_destino_post,puerto_origen_post,puerto_destino_post)
        select=request.POST['seleccion']
    
	if select=="1":
		w=1
        else:
		w=2
	
        #if not ip_origen_post and not ip_destino_post and not puerto_origen_post and not puerto_destino_post:
			#c=Flujos.objects.all()

        #else   ip_origen_post or not ip_destino_post or not puerto_origen_post or not puerto_destino_post:
			
			#c=Flujos.objects.filter(ip_origen=ip_origen_post,ip_destino=ip_destino_post,puerto_origen=puerto_origen_post,puerto_destino=puerto_destino_post)
		
	if ip_origen_post:
		p=p.filter(ip_origen=ip_origen_post)
	if ip_destino_post:
		p=p.filter(ip_destino=ip_destino_post)
	if puerto_origen_post:
		p=p.filter(puerto_origen=puerto_origen_post)
	if puerto_destino_post:
		p=p.filter(puerto_destino=puerto_destino_post)
	
        t=loader.get_template('entorno/fprobe_page.html')
	x=RequestContext(request,{'x':x,'p':p,'w':w,'s':s})
	return HttpResponse(t.render(x))     	
	

def inicio(request):
    #template = loader.get_template('entorno/prueb.html')
    return render(request,'entorno/inicio.html',{})

def des(request):
    #template = loader.get_template('entorno/prueb.html')
    return render(request,'entorno/des.html',{})

def flujos(request):
	
	x=Flujos.objects.all()
	t=loader.get_template('entorno/flujos.html')
	c=RequestContext(request,{'x':x})
	return HttpResponse(t.render(c))


      
def mrtg(request):
	
	p=MRTG.objects.all()
	t=loader.get_template('entorno/mrtg.html')
	c=RequestContext(request,{'p':p})
	return HttpResponse(t.render(c))
def sondas(request):
	
	t=loader.get_template('entorno/sondas.html')
	c=RequestContext(request,{})
	return HttpResponse(t.render(c))

def plotMRTG(request):
	print >>sys.stderr, 'Goodbye, cruel world!0'
	import matplotlib
	from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
	from matplotlib.figure import Figure
	from matplotlib.dates import DateFormatter
	
	

	
	
	fig = Figure()

	ax=fig.add_subplot(1,1,1)
	p=MRTG.objects.all()
	print >>sys.stderr, 'Goodbye, cruel world!1'

	
	ts = [sample.contador_bytes for sample in p]
	pkts = [sample.contador_paquetes for sample in p]

	print >>sys.stderr,ts
	print >>sys.stderr,pkts


	print >>sys.stderr, 'Goodbye, cruel world!2'
	

	ax.plot(ts, pkts)
	fig.patch.set_alpha(1)
	print >>sys.stderr, 'Goodbye, cruel world3!'
	#ax.set_xticks(ind + 0.5)
	#ax.set_xticklabels(names)

	ax.set_ylabel("Bytes")
	ax.set_xlabel("Time")

	#ax.set_xticklabels(names)

	title = u"Dynamically Generated Results MRTG:"
	ax.set_title(title)
	ax.patch.set_alpha(1.0)
	ax.patch.set_facecolor('none')

	#ax.grid(True)
	canvas = FigureCanvas(fig)
	response = HttpResponse(content_type='image/png')

	canvas.print_png(response)
	return response
	#return HttpResponse(t.render(c))


def filetransfer(request):
	
	p=Active_Measurement_file_transfer.objects.all()
	t=loader.get_template('entorno/filetransfer.html')
	c=RequestContext(request,{'p':p})
	return HttpResponse(t.render(c))



def packetpair(request):
	
	p=Active_Measurement_packet_pair.objects.all()
	t=loader.get_template('entorno/packetpair.html')
	c=RequestContext(request,{'p':p})
	return HttpResponse(t.render(c))

def packettrain(request):
	
	p=Active_Measurement_packet_train.objects.all()
	t=loader.get_template('entorno/packettrain.html')
	c=RequestContext(request,{'p':p})
	return HttpResponse(t.render(c))


def execute(request):
	s=Iperf.objects.all()
	ip_serv_iperf_post=request.POST['ip_servidor_iperf']
	print ip_serv_iperf_post
	#test=2
	#out23 = os.system('cd /home/tfm/mininet/netconf_05052017/ && ./listen -s "%s" -p 22 -l root' %ip_serv_iperf_post)
	args=shlex.split('/home/tfm/mininet/netconf_05052017/listen -s "%s" -p 22 -l root'%ip_serv_iperf_post)
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out1, err=proc.communicate()
	print out1
	out1=out1.split()
	out1=(out1[9], out1[10])
	j = " "
	out1=j.join(out1)
	t=loader.get_template('entorno/iperf_page.html')
	c=RequestContext(request,{'s':s,'out1':str(out1)})
	return HttpResponse(t.render(c))
	

def iperf(request):
	ip_serv_iperf_post=request.POST['ip_servidor_iperf']
	ip_cliente_iperf_post=request.POST['ip_cliente_iperf']
	args=shlex.split('/home/tfm/mininet/netconf_05052017/iperf -s "%s" -i "%s" -p 22 -l root' %(ip_cliente_iperf_post,ip_serv_iperf_post))
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out23, err=proc.communicate()	
	print out23
	out23=out23.split()

	ip_origen= out23[18]
	puerto_origen= out23[20]
	ip_dst= out23[23]
	puerto_dst= out23[25]
	interval= out23[176]
	interval=interval.split('-',1)
	inter= interval[1]
	print inter
	ancho_banda= out23[180]
	fecha = out23[195]

	#conexion BD
	Iperf.objects.create(timestamp = "%s" %(fecha) ,IP_origen=  "%s" %(ip_origen),IP_destino=  "%s" %(ip_dst),puerto_origen= "%s" %(puerto_origen),puerto_destino= "%s" %(puerto_dst),duracion =  "%s" %(inter),bandwitdh =  "%s" %(ancho_banda) )	
	#####
	l=Iperf.objects.latest('timestamp')
	s=Iperf.objects.all()
	t=loader.get_template('entorno/iperf_page.html')
	#c=RequestContext(request,{'out23':str(out23)})
	c=RequestContext(request,{'s':s,'l':l})
	return HttpResponse(t.render(c))

def formiperf(request):
	p=Iperf.objects.all()
	s=Iperf.objects.all()
	ip_origen_post=request.POST['ip_origen']
	ip_destino_post=request.POST['ip_destino']
 	puerto_origen_post=request.POST['puerto_origen']
 	puerto_destino_post=request.POST['puerto_destino']
	select=request.POST['seleccion']
    
	w=int(select)
		
	if ip_origen_post:
		p=p.filter(IP_origen=ip_origen_post)
	if ip_destino_post:
		p=p.filter(IP_destino=ip_destino_post)
	if puerto_origen_post:
		p=p.filter(puerto_origen=puerto_origen_post)
	if puerto_destino_post:
		p=p.filter(puerto_destino=puerto_destino_post)
	
        t=loader.get_template('entorno/iperf_page.html')
	x=RequestContext(request,{'s':s,'p':p,'w':w})
	return HttpResponse(t.render(x))


def siperf(request):
	s=Iperf.objects.all()
	args=shlex.split('/home/tfm/mininet/netconf_05052017/stop_iperf3 -s 10.0.0.3 -p 22 -l root')
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out2, err=proc.communicate()
	print out2
	out2=out2.split()
	out2=(out2[9], out2[10])
	j = " "
	out2=j.join(out2)
	t=loader.get_template('entorno/iperf_page.html')
	c=RequestContext(request,{'out2':str(out2),'s':s})
	return HttpResponse(t.render(c))

def fprobes(request):
	s=Flujos.objects.all()
	ip_post=request.POST['ip_fprobe']
	interfaz_post=request.POST['interfaz']
	puerto_post=request.POST['puerto']
	tiempo_post=request.POST['tiempo']
	ruta_post=request.POST['ruta']
	args=shlex.split('/home/tfm/mininet/netconf_05052017/fprobe_start -s "%s" -t "%s" -e "%s" -r "%s" -u "%s" -p 22 -l root'%(ip_post,tiempo_post,interfaz_post,ruta_post,puerto_post))
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out1, err=proc.communicate()
	print out1
	out1=out1.split()
	out1=(out1[9], out1[10])
	j = " "
	out1=j.join(out1)
	t=loader.get_template('entorno/fprobe_page.html')
	c=RequestContext(request,{'out1':str(out1),'s':s})
	return HttpResponse(t.render(c))

def fprober(request):
	ruta_post=request.POST['ruta']
	args=shlex.split('/home/tfm/mininet/netconf_05052017/fprobe_result -r "%s" -s 10.0.0.3 -p 22 -l root' %(ruta_post))
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out23, err=proc.communicate()
	print out23
	out23=out23.split()
	longitud=len(out23)
	print longitud
	print out23[10]
	print out23[9]
	#fechamin=time.mktime(datetime.datetime.strptime(out23[10], "%H:%M:%S.%f").timetuple())
	#fechamin_unix=time.mktime(out23[10].timetuple())
	#print fechamin
        #print fechamin_unix
        #print out23[12]
	#print out23[longitud-1]
	if(out23[18]=='G' or out23[18]=='M'):
		print ("se cumple")

	i=0
	#for i in range(0,longitud-1,10):
	while i < longitud-12:
		print i
		fecha_ini=time.mktime(datetime.datetime.strptime(out23[9+i], "%Y-%m-%d").timetuple())
		fecha_fin=time.mktime(datetime.datetime.strptime(out23[9+i], "%Y-%m-%d").timetuple())
		prueba= out23[13+i]
		prueba1=prueba.split(':',2)
		ip_org=prueba1[0]
		puerto_org=prueba1[1]
		prueba2= out23[15+i]
		prueba3=prueba2.split(':',2)
		ip_dst=prueba3[0]
		puerto_dst=prueba3[1]
		proto=out23[12+i]		
		num_paq=out23[16+i]
		num_bytes=out23[17+i]
		if(out23[18+i]=='G' or out23[18+i]=='M'):			
			i=i+1
		
		print ip_org
		print ip_dst
		print puerto_org
		print puerto_dst
		print num_bytes
		print num_paq
		print fecha_ini
		print proto 
		#conexion BD
		Flujos.objects.create(ip_origen = "%s" %(ip_org) ,ip_destino=  "%s" %(ip_dst),puerto_origen=  "%s" %(puerto_org),puerto_destino= "%s" %(puerto_dst),numero_bytes =  "%s" %(num_bytes),numero_paquetes =  "%s" %(num_paq),tiempo_inicio =  "%s" %(fecha_ini),tiempo_fin =  "%s" %(fecha_fin),protocolo_transporte =  "%s" %(proto))	
		#####
		i=i+10


	l=Flujos.objects.latest('tiempo_inicio')
	s=Flujos.objects.all()
	t=loader.get_template('entorno/fprobe_page.html')
	#c=RequestContext(request,{'out23':str(out23)})
	c=RequestContext(request,{'s':s,'l':l})
	return HttpResponse(t.render(c))

def sfprobe(request):
	s=Flujos.objects.all()
	args=shlex.split('/home/tfm/mininet/netconf_05052017/stop_fprobe -s 10.0.0.3 -p 22 -l root')
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out2, err=proc.communicate()
	print out2
	out2=out2.split()
	out2=(out2[9], out2[10])
	j = " "
	out2=j.join(out2)
	t=loader.get_template('entorno/fprobe_page.html')
	c=RequestContext(request,{'out2':str(out2),'s':s})
	return HttpResponse(t.render(c))	

def ifconfig(request):
	ip_ifconfig_post=request.POST['ip_ifconfig']
	args=shlex.split('/home/tfm/mininet/netconf_05052017/ifconfig -s "%s" -p 22 -l root'%(ip_ifconfig_post))
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out1, err=proc.communicate()
	print out1

	out1=out1.split()
	out1=("Interfaz:",out1[9],"Direccion MAC:",out1[13])
	s = " "
	out1=s.join(out1)

	t=loader.get_template('entorno/medidas.html')
	c=RequestContext(request,{'out1':str(out1)})
	return HttpResponse(t.render(c))

def properties(request):
	ip_properties_post=request.POST['ip_properties']
	args=shlex.split('/home/tfm/mininet/netconf_05052017/information -s "%s" -p 22 -l root'%(ip_properties_post))
	#print args
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out23, err=proc.communicate()
	print out23
	print out23[100]
	out23=out23.split()
	out2=("Arquitectura:",out23[10])
	s = " "
	out2=s.join(out2)
	out3=("Procesador:",out23[48],out23[49],out23[50],out23[51],out23[52],out23[53])
	s = " "
	out3=s.join(out3)

	out4=("Memoria total (MB):",out23[155],", Memoria usada(MB):",out23[156],", Memoria libre(MB):",out23[157],", Memoria compartida(MB):",out23[158],", Memoria buff/cache(MB):",out23[159],", Memoria disponible(MB):",out23[160])
	s = " "
	out4=s.join(out4)

	t=loader.get_template('entorno/medidas.html')
	c=RequestContext(request,{'out2':str(out2),'out3':str(out3),'out4':str(out4)})
	return HttpResponse(t.render(c))

def ping(request):
	ip_serv_iperf_post=request.POST['ip_servidor_iperf']
	ip_cliente_iperf_post=request.POST['ip_cliente_iperf']
	contador_post=request.POST['contador']
	contador_post=int(contador_post)
	args=shlex.split('/home/tfm/mininet/netconf_05052017/ping -s "%s" -i "%s" -c "%s" -p 22 -l root' %(ip_serv_iperf_post,ip_cliente_iperf_post,contador_post))
	proc=Popen(args,stdout=PIPE,stderr=PIPE)
	out23, err=proc.communicate()
	print out23	
	out23=out23.split()
	
	for i in range(0,contador_post*8,8):
		fecha= out23[44+i]
		packet_loss= out23[34+i]
		rtt= out23[42+i]

	packet_loss=packet_loss.split('%',1)
	rtt=rtt.split('/',3)
	rtt_min=rtt[0]
	rtt_avg=rtt[1]
	rtt_max=rtt[2]
	rtt_dev=rtt[3]
	ip_dst= out23[10]
	ip_origen= out23[5]

	#conexion BD
	Ping.objects.create(timestamp = "%s" %(fecha) ,IP_origen=  "%s" %(ip_origen),IP_destino=  "%s" %(ip_dst),Packetloss= "%s" %(packet_loss[0]),rtt_min= "%s" %(rtt_min),rtt_avg =  "%s" %(rtt_avg),rtt_max =  "%s" %(rtt_max),rtt_mdev =  "%s" %(rtt_dev))	
	#####		

	l=Ping.objects.latest('timestamp')
	s=Ping.objects.all()
	t=loader.get_template('entorno/ping_page.html')
	c=RequestContext(request,{'s':s,'l':l})
	#c=RequestContext(request,{'out23':out23})
	return HttpResponse(t.render(c))

def formping(request):
	p=Ping.objects.all()
	s=Ping.objects.all()
	ip_origen_post=request.POST['ip_origen']
	ip_destino_post=request.POST['ip_destino']
 	rtt_min_post=request.POST['rtt_min']
 	select=request.POST['seleccion']
    
	w=int(select)
       	print w
		
	if ip_origen_post:
		p=p.filter(IP_origen=ip_origen_post)
	if ip_destino_post:
		p=p.filter(IP_destino=ip_destino_post)
	if rtt_min_post:
		p=p.filter(rtt_min=rtt_min_post)

	
        t=loader.get_template('entorno/ping_page.html')
	x=RequestContext(request,{'s':s,'p':p,'w':w})
	return HttpResponse(t.render(x))
	 
def medidas(request):
    #template = loader.get_template('entorno/prueb.html')
    return render(request,'entorno/medidas.html',{})

def iperf_page(request):
	s=Iperf.objects.all()
	t=loader.get_template('entorno/iperf_page.html')
	c=RequestContext(request,{'s':s})
	return HttpResponse(t.render(c))

def ping_page(request):
	s=Ping.objects.all()
	t=loader.get_template('entorno/ping_page.html')
	c=RequestContext(request,{'s':s})
	return HttpResponse(t.render(c))

def fprobe_page(request):
	s=Flujos.objects.all()
	t=loader.get_template('entorno/fprobe_page.html')
	c=RequestContext(request,{'s':s})
	return HttpResponse(t.render(c))

      
