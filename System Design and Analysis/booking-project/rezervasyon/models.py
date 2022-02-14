from django.db import models
from django.utils import timezone
from django.contrib.auth.models import User
from django.db.models.signals import post_save
from django.dispatch import receiver
import datetime 





class Kullanici(models.Model):
    user = models.ForeignKey(User,on_delete=models.CASCADE)
   
    def __str__(self):
        return '{}'.format(self.user.username)

    
    adres = models.CharField(max_length = 500, db_column = "Adres", blank = True, verbose_name= ('Adres'))
    ulke = models.CharField(max_length = 20,db_column = "Ulke",blank = True, verbose_name=('Ülke'))
    sehir = models.CharField(max_length = 20,db_column = "Sehir",blank = True, verbose_name=('Şehir'))
    firma = models.CharField(max_length = 100, db_column = "Firma Adi", blank = True, verbose_name= ('Firma Adı'))
    kullanici_kayit_tarih = models.DateTimeField(db_column = "Kayit Tarihi",default = timezone.now, blank = True)
    
    

    class Meta:
        managed = True
        db_table = "Kullanici"

def create_profile(sender,**kwargs):
    user = kwargs['instance']
    if kwargs['created']:
        user_profile = Kullanici(user = user)
        user_profile.save()
    
post_save.connect(create_profile,sender = User)



class Gemi(models.Model):
    
    gemi_id = models.AutoField(primary_key = True,db_column = "Gemi_ID")
    isim = models.CharField(max_length = 20)
    boyut_choices = (
        (50000,'Handysize (35,000 - 50,000 DWT)'),
        (60000,'Supramax (50,000 - 60,000 DWT)'),
        (80000,'Panamax (60,000 - 80,000 DWT)')
    )
    boyut = models.IntegerField(choices= boyut_choices,default = 0)
    gemi_kayit_tarih = models.DateTimeField(default = timezone.now)
    
    kullanici = models.ForeignKey("Kullanici",on_delete = models.CASCADE,blank = True, null = False)

    def __str__(self):
        return '{} #{}'.format(self.isim,self.gemi_id)

    class Meta:
        managed = True
        db_table = "Gemi"


class Yer(models.Model):

    yer_id = models.AutoField(db_column = "Yer_ID",primary_key = True)
    tarih = models.DateField(db_column = 'Tarih')
    gemiler = []
    kapasite = models.IntegerField(default = 40000000,blank = True,null = True)

    

    class Meta:
        managed = True
        db_table = "Yer"

class Rezervasyon(models.Model):

    rezervasyon_id = models.AutoField(db_column = "Rezervasyon_ID",primary_key = True)
    gemi = models.ForeignKey("Gemi",on_delete = models.CASCADE,null = True)
    kullanici = models.ForeignKey("Kullanici",on_delete = models.CASCADE,db_column = "Kullanici_ID",null = True)
    
    
    tarih_giris = models.DateField(db_column = "Giris_Tarihi",verbose_name = "Giriş Tarihi")
    tarih_cikis = models.DateField(db_column = "Cikis_Tarihi",verbose_name="Çıkış Tarihi")
    fiyat = models.IntegerField(default = 0)

    class Meta:
        managed = True
        db_table = "Rezervasyon"

class Settings(models.Model):

    sozlesme_onay = models.BooleanField(default = False,verbose_name=("Sözleşme Onay"))


