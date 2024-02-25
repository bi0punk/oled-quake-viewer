from rest_framework import serializers
from .models import Sismo



class SismoSerializer(serializers.ModelSerializer):
    class Meta:
        model = Sismo
        fields = '__all__'