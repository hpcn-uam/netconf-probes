from django.conf.urls import patterns, url

from entorno import views

urlpatterns = patterns('',
    # ex: /polls/
    url(r'^$', views.index, name='index'),
    url(r'^formini/$', views.formini, name='formini'),
    url(r'^formflujos/$', views.formflujos, name='formflujos'),
    url(r'^formflujos_fprobe/$', views.formflujos_fprobe, name='formflujos_fprobe'),
    url(r'^formreg/$', views.formreg, name='formreg'),
    url(r'^inicio/$', views.inicio, name='inicio'),
    url(r'^flujos/$', views.flujos, name='flujos'),
    url(r'^mrtg/$', views.mrtg, name='mrtg'),
    url(r'^sondas/$', views.sondas, name='sondas'),
    url(r'^mrtg/resultMRTG.png$', views.plotMRTG),
    url(r'^filetransfer/$', views.filetransfer, name='filetransfer'),
    url(r'^packetpair/$', views.packetpair, name='packetpair'),
    url(r'^packettrain/$', views.packettrain, name='packettrain'),
    url(r'^des/$', views.des, name='des'),
    url(r'^execute/$', views.execute, name='execute'),
    url(r'^iperf/$', views.iperf, name='iperf'),
    url(r'^siperf/$', views.siperf, name='siperf'),
    url(r'^medidas/$', views.medidas, name='medidas'),
    url(r'^fprobes/$', views.fprobes, name='fprobes'),
    url(r'^fprobers/$', views.fprober, name='fprober'),
    url(r'^ifconfig/$', views.ifconfig, name='ifconfig'),	
    url(r'^properties/$', views.properties, name='properties'),	
    url(r'^ping_page/$', views.ping_page, name='ping_page'),
    url(r'^iperf_page/$', views.iperf_page, name='iperf_page'),
    url(r'^fprobe_page/$', views.fprobe_page, name='fprobe_page'),
    url(r'^formiperf/$', views.formiperf, name='formiperf'),
    url(r'^ping/$', views.ping, name='ping'),
    url(r'^formping/$', views.formping, name='formping'),
    url(r'^sfprobe/$', views.sfprobe, name='sfprobe'),	

    
      
)
