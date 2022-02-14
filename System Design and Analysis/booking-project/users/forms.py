
from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from rezervasyon.models import Kullanici,Gemi,Rezervasyon,Settings
from bootstrap_datepicker_plus import DatePickerInput
from datetime import date

class UserRegisterForm(UserCreationForm):
    email = forms.EmailField(max_length = 254)
    class Meta:
        model = User
        
        fields = ('username',"password1","password2", 'email','first_name','last_name')
        
        #fields = ['username', 'kullanici_adi', 'kullanici_soyadi','email','password1',"password2"]

class KullaniciForm(forms.ModelForm):
    adres = forms.CharField(max_length = 500)
    sehir = forms.CharField(max_length = 20)
    class Meta:
        model = Kullanici
        fields = ['adres',"ulke","sehir",'firma']

class UserLoginForm(forms.ModelForm):


    class Meta:
        model = Kullanici
        widgets = {
        'password' : forms.PasswordInput
        }
        fields = []

class RegisterGemiForm(forms.ModelForm):

    class Meta:
        model = Gemi
        fields = ['isim','boyut']



class DateInput(forms.DateInput):
    input_type = "date"


class RezervasyonForm(forms.ModelForm):
    
    def __init__(self, *args, **kwargs):
        self.user = kwargs.pop("user")
        super(RezervasyonForm, self).__init__(*args, **kwargs)
        self.users = Kullanici.objects.filter(user = self.user)
        self.current_user = self.users[1]
        self.gemis = Gemi.objects.filter(kullanici_id = self.current_user.id)
        self.fields['gemi'].queryset = self.gemis
        #self.fields['kullanici'] = self.current_user
        
    
        
    gemi = forms.ModelChoiceField(queryset=None, widget=forms.Select, required=True)    
        
    
    
    tarih_giris = forms.DateField(label = "Giriş Tarihi",input_formats=['%d/%m/%y'],initial = date.today)
    tarih_cikis = forms.DateField(label = "Çıkış Tarihi",input_formats=['%d/%m/%y'],initial = date.today)
    
    
    class Meta:
        model = Rezervasyon
        fields = ['tarih_giris','tarih_cikis','gemi']
        
        """
        widgets = {
            'tarih_giris' : DateInput(attrs={'class':'form-control', 'type':'date'}),
            'tarih_cikis' : DateInput(attrs={'class':'form-control', 'type':'date'})
        }
        """

class SettingsForm(forms.ModelForm):

    sozlesme_onay = forms.BooleanField(label = 'Kişisel verilerimin işlenmesini kabul ediyorum.')

    class Meta:
        model = Settings
        fields = ['sozlesme_onay']
        

        
        
        
        