from django.urls import path
from .views import ListaSismos, DetalleSismo  # Importación correcta desde views.py
from . import views
from .views import latest_earthquake

urlpatterns = [
    path('sismos/', ListaSismos.as_view(), name='lista-sismos'),
    path('sismos/<int:pk>/', DetalleSismo.as_view(), name='detalle-sismo'),
    path('', views.api, name='api'),
    path('sismo/latest/', latest_earthquake, name='latest_earthquake'),
]