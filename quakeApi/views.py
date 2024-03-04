from django.http import HttpResponse
from django.shortcuts import render

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


# Others Views
def api(request):
    return render(request, 'quakeApi/index.html')