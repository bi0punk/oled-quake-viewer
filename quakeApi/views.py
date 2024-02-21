from django.shortcuts import render

# Create your views here.
from rest_framework import generics
from .models import Libro
from .serializers import LibroSerializer, SismoSerializer
from .models import Sismo


class ListaLibros(generics.ListCreateAPIView):
    queryset = Libro.objects.all()
    serializer_class = LibroSerializer

class DetalleLibro(generics.RetrieveUpdateDestroyAPIView):
    queryset = Libro.objects.all()
    serializer_class = LibroSerializer



class ListaSismos(generics.ListCreateAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer

class DetalleSismo(generics.RetrieveUpdateDestroyAPIView):
    queryset = Sismo.objects.all()
    serializer_class = SismoSerializer
