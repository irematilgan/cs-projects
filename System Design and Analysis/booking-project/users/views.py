from django.shortcuts import render,redirect
from django.contrib.auth.forms import UserCreationForm, AuthenticationForm
from django.contrib import messages
from .forms import UserRegisterForm, UserLoginForm, KullaniciForm, RezervasyonForm, RegisterGemiForm,SettingsForm
from django.contrib.auth import authenticate, login, logout
from django.forms.formsets import formset_factory
from django.http import HttpResponseRedirect, HttpResponse
from django.urls import reverse
from rezervasyon.models import Gemi,Kullanici,Rezervasyon,Yer
from django.views import generic
from datetime import datetime

#Kayıt Fonksiyonu
def register(request):

    if request.method == "POST":
        
        
        form = UserRegisterForm(request.POST)
        form2 = KullaniciForm(request.POST)
        form3 = SettingsForm(request.POST)
        
        # Form verisi geçerli mi kontrol et
        if all([form.is_valid(),form2.is_valid()]): 
            #Form bilgisini modele kayıt et
           
            user = form.save() 
            form2.instance.user = user
            info = form2.save()
            form3.instance.info = info
            form3.save()            
           
                    
            messages.success(request,'Hesap Oluşturuldu')
            return redirect("giris")
    else:
        form = UserRegisterForm()
        form2 = KullaniciForm()
        form3 = SettingsForm()
  
   
    return render(request,'users/register.html',{'form' : form, 'formset' : form2,'formset2':form3})

#Gemi Ekleme Fonksiyonu
def gemiEkle(request):
    if request.method == 'POST':
        form = RegisterGemiForm(request.POST)

        if form.is_valid():
            isim = form.cleaned_data['isim']
            boyut = form.cleaned_data['boyut']
            current_user = Kullanici.objects.filter(user_id = request.user.id)
            #Gemi objesi oluşturulur
            instance = Gemi(isim = isim, kullanici = current_user[1],boyut = boyut)
            instance.save()
            
            messages.success(request,'Gemi Kaydedildi')
            return redirect('rez-home')

    else:
        form = RegisterGemiForm(request.POST)

    return render(request,'users/gemiekle.html',{'form':form})



def rezervasyon(request):
    giris = None
    
    if request.method == 'POST':
        
        form = RezervasyonForm(data = request.POST,user = request.user)
        
        if form.is_valid():
            
            giris = form.cleaned_data['tarih_giris']
            cikis = form.cleaned_data['tarih_cikis']
            gemi = form.cleaned_data['gemi']
            if checkDate(giris,cikis,gemi):
                current_user = Kullanici.objects.filter(user = request.user)
                fatura = calculateBill(giris,cikis,gemi)
                #Rezervasyon oluşturulur
                instance = Rezervasyon(gemi = gemi,kullanici = current_user[1], tarih_giris = giris, tarih_cikis = cikis, fiyat = fatura)
                instance.save()
                messages.success(request,'Rezervasyon Başarılı')
                return redirect('rez-home')


            messages.error(request,"Rezervasyon Başarısız!..Uygun Tarihleri Seçiniz")
        
    
    else:
        form = RezervasyonForm(user = request.user)
        

    
   
    
    return render(request, 'rezervasyon/rezervasyon_form.html',context = {
            'form' : form,
    })


#Tarihlerin kontrolü için kullanılan fonksiyon
def checkDate(giris,cikis,gemi):
    
    today = datetime.now()
    date1 = giris.strftime("%d %m %Y")
    date2 = cikis.strftime("%d %m %Y")
    today_date = today.strftime("%d %m %Y")
    arr1 = date1.strip(" ")
    arr2 = date2.strip(" ")
    arr3 = today_date.strip(" ")

    #Giriş ve çıkış tarihleri arasındaki farklar bulunur
    dif_day = int(arr2[:2])-int(arr1[0:2])
    dif_month = int(arr2[3:5])-int(arr1[3:5])
    dif_year = int(arr2[6:])-int(arr1[6:])

    #Eğer çıkış tarihi giriş tarihinden önceyse talebi kabul etme
    if dif_day < 0 or dif_month < 0 or dif_year < 0:
        return 0

    #Eğer giriş veya çıkış tarihi bugünden önceki bir tarih ise talebi kabul etme
    #(Geçmişteki bir tarihi kabul etme)

    condition1 = int(arr1[6:]) - int(arr3[6:])
    condition2 = int(arr1[4]) + int(arr1[3])*10 - (int(arr3[4]) + int(arr3[3])*10)
    condition3 = int(arr1[:2]) - int(arr3[:2])
    total1 = condition1*365+condition2*30+condition3
    print(arr1)
    if total1 < 0:
        return 0

    condition12 = int(arr2[6:]) - int(arr3[6:])
    condition22 = int(arr2[4]) + int(arr2[3])*10 - (int(arr3[4]) + int(arr3[3])*10)
    condition32 = int(arr2[:2]) - int(arr3[:2])
    
    total2 = condition12*365+condition22*30+condition32
    print(arr1)
    if total2 < 0:
        return 0



    #Daha önce o tarih için rezervasyon yapılıp yapılmadığı kontrol edilir
    try:
        record1 = Yer.objects.get(tarih = giris)
    except:
        record1 = None

    try:
        record2 = Yer.objects.get(tarih = cikis)
    except:
        record2 = None

    #Eğer bu tarih aralığında hiç rezervasyon yapılmamışsa, yeni kayıtlar oluşturulur
    if record1 == None and record2 == None:
        yer1 = Yer(tarih = giris)
        yer1.gemiler.append(gemi)
        yer1.kapasite -= gemi.boyut #Kayıt edilen gemi, mevcut kapasiteden çıkartılır
        yer1.save()
        
        yer2 = Yer(tarih = cikis)
        yer2.gemiler.append(gemi)
        yer2.kapasite -= gemi.boyut
        yer2.save()
        return 1
    elif record1 != None and record2 != None: #Eğer iki tarihte de daha önce kayıt yapıldıysa 
        #Tarihlerdeki kapasiteler kontrol edilir, doluysa talep kabul edilmez
        if record1.kapasite - gemi.boyut < 0 or record2.kapasite - gemi.boyut < 0:
            return 0
        #Rezerve edilmeye çalışılan gemi, daha önce o tarihte kayıt ettirilmişse talep kabul edilmez
        if gemi in record1.gemiler:
            return 0

        record1.gemiler.append(gemi)
        record1.kapasite -= gemi.boyut
        record1.save()
        record2.gemiler.append(gemi)
        record2.kapasite -= gemi.boyut
        record2.save()
        return 1
    elif record2 == None:
        old_yer = record1
    elif record1 == None:
        old_yer = record2

    if old_yer.kapasite - gemi.boyut < 0:
        return 0
    if gemi in old_yer.gemiler:
        return 0

    yer = Yer(tarih = cikis)
    yer.gemiler.append(gemi)
    yer.kapasite -= gemi.boyut
    yer.save()
    old_yer.gemiler.append(gemi)
    old_yer.kapasite -= gemi.boyut
    old_yer.save()
    
    return 1    #Talep kabul edilir
    
#Fatura Hesaplama Fonksiyonu
def calculateBill(giris,cikis,gemi):
    
    date1 = giris.strftime("%d %m %Y")
    date2 = cikis.strftime("%d %m %Y")
    
    arr1 = date1.strip(" ")
    arr2 = date2.strip(" ")

    #İki tarih arasındaki fark alınır
    dif_day = int(arr2[:2])-int(arr1[0:2])
    dif_month = int(arr2[3:5])-int(arr1[3:5])
    dif_year = int(arr2[6:])-int(arr1[6:])
    
    #Fatura hesaplanır
    bill = (dif_day+dif_month*30+dif_year*365)*int((gemi.boyut)/20)
   
    return bill

#Giriş Fonksiyonu
def signin(request):
    
    if request.method == 'POST':
        
        username = request.POST.get('username')
        password = request.POST.get('password')
        user = authenticate(username=username, password=password)
        
        if user:
            if user.is_active:
                login(request,user)
                return redirect('rez-about')
            else:
                return HttpResponse("Hesabınız aktif değil.")
        else:
            print("Birisi hesabınıza girmeye çalıştı ve başarısız oldu.")
            return HttpResponse("Yanlış şifre veya kullanıcı adı.")

  
    return render(request,'users/login.html',{})

#Profil Gösterme Fonksiyonu
def profil(request):
    #Giriş yapan kullanıcının bilgileri alınır
    current_user = Kullanici.objects.filter(user = request.user)
    gemis = Gemi.objects.filter(kullanici = current_user[1])


    try:
        rezervasyon = Rezervasyon.objects.filter(kullanici = current_user[1])
    except:
        rezervasyon = None

    isim = request.user.get_full_name()
    
    context = {
        'isim' : isim,
        'kullanici' : current_user[1],
        'gemis' : gemis,
        'rezervasyons' : rezervasyon
        
    }
    

    return render(request,'users/profil.html', context = context)