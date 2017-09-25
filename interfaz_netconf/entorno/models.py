# Create your models here.

from django.db import models

  		
          
class Active_Measurement_file_transfer(models.Model):
    capacity = models.DecimalField(max_digits=10,decimal_places=3)
    available_bandwidth = models.DecimalField(max_digits=10,decimal_places=3)
    one_way_delay = models.DecimalField(max_digits=10,decimal_places=3)
    round_trip_time = models.DecimalField(max_digits=10,decimal_places=3)
    jitter = models.DecimalField(max_digits=10,decimal_places=3)
    packet_loss  =  models.IntegerField()
        
class Active_Measurement_packet_pair(models.Model):
    capacity = models.DecimalField(max_digits=10,decimal_places=3)
    available_bandwidth = models.DecimalField(max_digits=10,decimal_places=3)
    one_way_delay = models.DecimalField(max_digits=10,decimal_places=3)
    round_trip_time = models.DecimalField(max_digits=10,decimal_places=3)
    jitter = models.DecimalField(max_digits=10,decimal_places=3)
    packet_loss  =  models.IntegerField()

class Active_Measurement_packet_train(models.Model):
    capacity = models.DecimalField(max_digits=10,decimal_places=3)
    available_bandwidth = models.DecimalField(max_digits=10,decimal_places=3)
    one_way_delay = models.DecimalField(max_digits=10,decimal_places=3)
    round_trip_time = models.DecimalField(max_digits=10,decimal_places=3)
    jitter = models.DecimalField(max_digits=10,decimal_places=3)
    packet_loss  =  models.IntegerField()


class Flujos(models.Model):
    ip_origen = models.CharField(max_length=16)
    ip_destino = models.CharField(max_length=16)
    puerto_origen= models.IntegerField()
    puerto_destino= models.IntegerField()
    protocolo_transporte = models.CharField(max_length=5)
    numero_bytes = models.DecimalField(max_digits=10,decimal_places=3)
    numero_paquetes= models.IntegerField()
    tiempo_inicio =  models.DecimalField(max_digits=16,decimal_places=6)
    tiempo_fin = models.DecimalField(max_digits=16,decimal_places=6)
       

class MRTG(models.Model):
    contador_bytes= models.DecimalField(max_digits=15,decimal_places=3)
    contador_paquetes= models.IntegerField()
    flujos_concurrentes= models.IntegerField()
    timestamp =  models.DecimalField(max_digits=16,decimal_places=6)

class Usuario(models.Model):
    usuario=  models.CharField(max_length=16)
    email=  models.CharField(max_length=32)
    contrasena=  models.CharField(max_length=16)
 
class Iperf(models.Model):
    timestamp =  models.DecimalField(max_digits=16,decimal_places=0)
    IP_origen=  models.CharField(max_length=32)
    IP_destino=  models.CharField(max_length=32)
    puerto_origen= models.IntegerField()
    puerto_destino= models.IntegerField()
    duracion =  models.DecimalField(max_digits=5,decimal_places=2)
    bandwitdh =  models.DecimalField(max_digits=5,decimal_places=2)

class Ping(models.Model):
    timestamp =  models.DecimalField(max_digits=16,decimal_places=0)
    IP_origen=  models.CharField(max_length=32)
    IP_destino=  models.CharField(max_length=32)
    Packetloss =  models.DecimalField(max_digits=8,decimal_places=2)
    rtt_min =  models.DecimalField(max_digits=4,decimal_places=3)
    rtt_avg =  models.DecimalField(max_digits=4,decimal_places=3)
    rtt_max =  models.DecimalField(max_digits=4,decimal_places=3)
    rtt_mdev =  models.DecimalField(max_digits=4,decimal_places=3)
	
      

   

