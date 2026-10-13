from django.urls import path

from . import views
from .views import (  # Importación correcta desde views.py
    DetalleSismo,
    ListaSismos,
    latest_earthquake,
)

urlpatterns = [
    path('sismos/', ListaSismos.as_view(), name='lista-sismos'),
    path('sismos/<int:pk>/', DetalleSismo.as_view(), name='detalle-sismo'),
    path('', views.api, name='api'),
    path('sismo/latest/', latest_earthquake, name='latest_earthquake'),
]