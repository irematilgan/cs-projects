
from .models import Gemi,Rezervasyon,Kullanici
from django import forms
from django.contrib.auth import authenticate
from django.contrib.auth.forms import AuthenticationForm
from django.utils.translation import ugettext_lazy as _
from django.conf import settings
from django.contrib.auth.forms import UserCreationForm

class SignUpForm(UserCreationForm):
    

    class Meta:
        model = Kullanici
        fields = ('username', 'kullanici_adi', 'kullanici_soyadi' )
