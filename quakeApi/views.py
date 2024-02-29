from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.
from rest_framework import generics
from .serializers import SismoSerializer
from .models import Sismo


class ListaSismos(generics.ListCreateAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer

class DetalleSismo(generics.RetrieveUpdateDestroyAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer




def mi_nueva_vista(request):
    return HttpResponse("Hola, esta es mi nueva vista!")