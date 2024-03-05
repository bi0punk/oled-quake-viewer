from rest_framework import serializers
from .models import Sismo

class SismoSerializer(serializers.ModelSerializer):
    fecha_local = serializers.DateTimeField(format='%Y-%m-%dT%H:%M:%S')
    class Meta:
        model = Sismo
        fields = '__all__'