from django.http import HttpResponse
from django.shortcuts import render
from django.http import JsonResponse
from rest_framework.views import APIView
from rest_framework.response import Response
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


# Others Views

def api(request):
    # Obteniendo los últimos 7 objetos Sismo, ordenados por 'fecha_creacion' descendente
    sismos = Sismo.objects.all().order_by('-fecha_local')[:7]
    # Serializando los datos
    serializer = SismoSerializer(sismos, many=True)
    # Devolviendo los datos serializados como una respuesta JSON
    return JsonResponse(serializer.data, safe=False)