from django.http import HttpResponse
from django.shortcuts import render
from django.http import JsonResponse
from rest_framework.views import APIView
from rest_framework.response import Response
from django.utils import timezone

# Create your views here.
from rest_framework import generics
from .serializers import SismoSerializer
from .models import Sismo
from rest_framework.renderers import JSONRenderer
from django.core.serializers.json import DjangoJSONEncoder
import json

class ListaSismos(generics.ListCreateAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer

class DetalleSismo(generics.RetrieveUpdateDestroyAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer

def api(request):
    # Obteniendo los últimos 7 objetos Sismo, ordenados por 'fecha_creacion' descendente
    sismos = Sismo.objects.all().order_by('-fecha_local')[:7]
    # Serializando los datos
    serializer = SismoSerializer(sismos, many=True)
    # Devolviendo los datos serializados como una respuesta JSON
    return JsonResponse(serializer.data, safe=False)

def latest_earthquake(request):
    # Obtener el último registro de sismo ordenado por fecha_utc
    latest_sismo = Sismo.objects.latest('fecha_utc')
    
    # Preparar la respuesta con los datos del sismo
    response_data = {
        'fecha_local': latest_sismo.fecha_local.strftime('%Y-%m-%d %H:%M:%S'),
        'fecha_utc': latest_sismo.fecha_utc.strftime('%Y-%m-%d %H:%M:%S'),
        'ubicacion': latest_sismo.ubicacion,
        'latitud': latest_sismo.latitud,
        'longitud': latest_sismo.longitud,
        'profundidad': latest_sismo.profundidad,
        'magnitud': latest_sismo.magnitud
    }
    
    # Devolver la respuesta en formato JSON
    return JsonResponse(response_data)

    
    # Devolver la respuesta en formato JSON
    return JsonResponse(response_data)