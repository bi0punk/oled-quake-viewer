# quake.py
#!/usr/bin/env python3
#
#  quake.py
#  
#  Copyright 2024 
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  

import requests
from bs4 import BeautifulSoup
import datetime
import re
import csv

def obtener_sismos():
    fecha_actual = datetime.datetime.now()
    formato_url = fecha_actual.strftime('%Y/%m/%Y%m%d')
    url = f'https://www.sismologia.cl/sismicidad/catalogo/{formato_url}.html'

    try:
        response = requests.get(url)
        response.raise_for_status()  # Lanza una excepción para estados 4xx/5xx
    except requests.exceptions.RequestException as e:
        print(f"Error al realizar la solicitud HTTP: {e}")
        return []

    soup = BeautifulSoup(response.text, 'html.parser')
    table = soup.find('table', class_='sismologia detalle')
    sismos_data = []

    if not table:
        print("No se encontró la tabla de sismos en la página web.")
        return sismos_data

    print("Tabla de sismos encontrada, procesando datos...")
    for row in table.find_all('tr')[1:]:
        cols = [ele.text.strip() for ele in row.find_all('td')]
        if cols:
            fecha_local = datetime.datetime.strptime(cols[0][:19], '%Y-%m-%d %H:%M:%S')
            lugar = cols[0][19:].strip()
            latitud = float(cols[2].split(' ')[0])
            longitud = float(cols[2].split(' ')[1])
            profundidad = float(re.search(r'\d+(\.\d+)?', cols[3]).group())
            magnitud = float(re.search(r'\d+(\.\d+)?', cols[4]).group())

            sismos_data.append({
                'Fecha Local': fecha_local,
                'Lugar': lugar,
                'Fecha UTC': cols[1],  # Considera ajustar esto según necesidades
                'Latitud': latitud,
                'Longitud': longitud,
                'Profundidad': profundidad,
                'Magnitud': magnitud,
            })
    guardar_sismos_csv(sismos_data)
    return sismos_data

def guardar_sismos_csv(sismos_data, archivo='sismos_respaldo.csv'):
    # Definir los nombres de las columnas basados en las claves del diccionario
    columnas = ['Fecha Local', 'Lugar', 'Fecha UTC', 'Latitud', 'Longitud', 'Profundidad', 'Magnitud']
    
    try:
        with open(archivo, mode='w', newline='', encoding='utf-8') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=columnas)
            
            writer.writeheader()
            for sismo in sismos_data:
                writer.writerow(sismo)
                
        print(f"Datos de sismos guardados en {archivo}")
    except Exception as e:
        print(f"Error al guardar los datos de sismos en CSV: {e}")