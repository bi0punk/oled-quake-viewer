import requests
from .models import Quake
from django.http import JsonResponse
from rest_framework.views import APIView
from rest_framework.response import Response
from .serializers import DatoSerializer
import requests
from bs4 import BeautifulSoup
import datetime
import csv
import json
import pandas as pd
from django.db import transaction


def recolectar_datos_sismologicos():
    fecha_actual = datetime.datetime.now()
    formato_url = fecha_actual.strftime('%Y/%m/%Y%m%d')
    url = f'https://www.sismologia.cl/sismicidad/catalogo/{formato_url}.html'
    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.text, 'html.parser')
        table = soup.find('table', class_='sismologia detalle')

        if table:
            with transaction.atomic():  # Utilizar una transacción para la coherencia de la base de datos
                for row in table.find_all('tr')[1:]:
                    try:
                        cols = [ele.text.strip() for ele in row.find_all('td')]
                        if cols:
                            # Suponiendo que tu modelo Dato tiene los campos apropiados.
                            Quake.objects.create(
                                fecha_local=cols[0][:19],
                                lugar=cols[0][19:].strip(),
                                fecha_utc=cols[1],
                                latitud=cols[2].split(' ')[0],
                                longitud=cols[2].split(' ')[1] if len(cols[2].split(' ')) > 1 else '',
                                profundidad=cols[3],
                                magnitud=cols[4]
                            )
                    except Exception as e:
                        # Considera manejar o registrar el error
                        continue
            print("Datos guardados correctamente.")
        else:
            print("Tabla no encontrada.")
    else:
        print(f"Error en la petición web: {response.status_code}")



class UltimoDato(APIView):
    def get(self, request, *args, **kwargs):
        try:
            ultimo_dato = Quake.objects.latest('fecha_local')  # Asegúrate de tener un campo de fecha adecuado
            serializer = DatoSerializer(ultimo_dato)
            return Response(serializer.data)
        except Quake.DoesNotExist:
            return Response({'error': 'No hay datos disponibles.'}, status=404)
        

class RecolectarDatosSismologicos(APIView):
    def get(self, request, *args, **kwargs):
        try:
            recolectar_datos_sismologicos()
            return Response({"mensaje": "Recolección de datos iniciada correctamente."})
        except Exception as e:
            return Response({"error": str(e)}, status=500)
        