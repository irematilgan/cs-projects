import selenium
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from bs4 import BeautifulSoup
import time
import requests
import pandas as pd
from selenium.webdriver.support.wait import WebDriverWait
from tqdm import tqdm
import json

def mine(search_keys):
    print("================ MINING FOR ELEMANNET ================")
    print("\n")
    BASE_URL = 'https://www.eleman.net/is-ilanlari/'
    # search_keys = ['yazılım mühendisi', 'iş analisti', 'pazarlama', 'satış', 'şoför']
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("window-size=1920x1080")
    driver = webdriver.Chrome(chrome_options = chrome_options)
    driver.get(BASE_URL)
    driver.set_window_size(1920, 1080)
    time.sleep(1)

    jobs_list = []
    for search_key in search_keys:
        html = driver.page_source
        pg = BeautifulSoup(html)
        time.sleep(3)
        # driver.find_element_by_class_name("vl-notif-delay").click()
        driver.find_element_by_id("aranan-kelime").send_keys(search_key)
        time.sleep(1)
        driver.find_element_by_xpath("//button[@class='c-button c-button--warning']").click()
        time.sleep(1)
        html_searched = driver.page_source
        pg_searched = BeautifulSoup(html_searched)
        #print(pg_searched)
        for page in range(1):
            jobs = pg_searched.find_all("a",{"class" : "l-flex__left"})
            driver.find_element_by_xpath("//div[@class='c-box c-box--small c-box--animation@lg-up u-gap-bottom ']").text
            try:
                for job in jobs:
                    job_dict = {}
                    job_dict['keyword'] = search_key
                    job_dict['src'] = 'elemannet'
                    job_link = job.attrs['href']
                    job_dict['url'] = job_link
                    driver.get(job_link)
                    #driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
                    time.sleep(1)
                    html_job = driver.page_source
                    pg_job = BeautifulSoup(html_job)
                    #job_title = driver.find_elements_by_xpath("//h1[@class='h3 u-clear-gap-bottom u-text-ilan-baslik']")
                    job_title = pg_job.find("h1",{"class" : "h3 u-clear-gap-bottom u-text-ilan-baslik"}).attrs['title']
                    skipper = 0
                    for temp in jobs_list:
                        if(temp['title']==job_title):
                            skipper=1
                    if(skipper==1):
                        continue

                    print(job_title)

                    job_dict['title'] = job_title
                    job_dict['tags'] = []
                    job_dict['general-desc'] = driver.find_element_by_xpath("//div[@class='u-font-size-sm']").text

                    tags = driver.find_elements_by_xpath("//span[@class='sbp_yazi']/a")
                    for tag in tags:
                        job_dict['tags'] = tag.text


                    # aday kriterleri bolumu
                    job_dict['candidate-criterias'] = []

                    jobs_list.append(job_dict)
                    time.sleep(1)
                    driver.back()


            except Exception as e:
                print(e)

            time.sleep(2)

        driver.get(BASE_URL)
        time.sleep(2)

    # print(jobs_list)

    with open('jobs_elemannet.json', 'w', encoding='utf8') as json_file:
        json.dump(jobs_list, json_file, ensure_ascii=False, indent = 4)

    driver.close()

    print("================ MINING COMPLETED ================")
