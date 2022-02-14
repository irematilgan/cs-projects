import selenium
from selenium import webdriver
from bs4 import BeautifulSoup, BeautifulStoneSoup
import time
import requests
import pandas as pd
from tqdm import tqdm

def applySearch(driver):
    # Arama Butonu
    driver.find_element_by_xpath("//button[@data-test='apply-button']").click()
    time.sleep(2)

def refreshPage(url, driver):
    driver.get(url)
    driver.maximize_window()
    time.sleep(3)

BASE_URL = 'https://www.kariyer.net/is-ilanlari'

# Sayfayi yukle
driver = webdriver.Chrome()
refreshPage(BASE_URL,driver)

html = driver.page_source
pg = BeautifulSoup(html)
time.sleep(3)
# driver.find_element_by_class_name("vl-notif-delay").click()

# Sehir secme
selected_cities = ['Ankara', 'İstanbul (Tümü)', 'İzmir']
driver.find_element_by_xpath("//span[@data-test='city-title']").click()
time.sleep(2)

cities = driver.find_elements_by_xpath("//*[@id='__BVID__50___BV_modal_body_']/div/div/div/div/div/label")
city_labels = driver.find_elements_by_xpath("//*[@id='__BVID__50___BV_modal_body_']/div/div/div/div/div/label/div")
print(len(city_labels))
for i, city_label in enumerate(city_labels):
    if city_label.text in selected_cities:
        cities[i].click()

driver.find_element_by_xpath("//*[@id='__BVID__50___BV_modal_header_']/div/span").click() # diyalog penceresini kapa
time.sleep(2)

# Sehirleri arat
applySearch(driver)

num_total_jobs = driver.find_element_by_xpath("//*[@class='t-6 text-secondary mb-3 search-result-section']").text
print("TOPLAM IS = ", num_total_jobs)

# Sayfayi yenile
# refreshPage(BASE_URL,driver)

driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
time.sleep(2)

# Tarih secme : 7 secenek mevcut (Projede 1,4,5,6 indexleri -> bugun, son 3-7-15 gun kullanildi)
date_index = [1,4,5,6]
chosen_date = -1 # secilen date index
if chosen_date == -1:
    num_jobs_date_dict = {}
    date_labels = driver.find_elements_by_xpath("//*[@id='__BVID__242']/div/input")
    print(date_labels)
    print(len(date_labels))
    for date_label in date_labels:
        #print(driver.find_elements_by_xpath("//*[@id='__BVID__236']/div/label"))
        date_label.click()
        applySearch(driver)
        num_jobs_date_dict[str(i)] = driver.find_element_by_xpath("//*[@class='t-6 text-secondary mb-3 search-result-section']").text
        driver.implicitly_wait(3)
    print(num_jobs_date_dict)
else:
    driver.find_elements_by_xpath("//*[@id='__BVID__236']/div/label")[date_index[chosen_date]].click()
    num_jobs_date = driver.find_element_by_xpath("//*[@class='t-6 text-secondary mb-3 search-result-section']").text


time.sleep(2)


time.sleep(5)
driver.close()