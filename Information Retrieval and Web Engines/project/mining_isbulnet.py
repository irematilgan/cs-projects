import selenium
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from bs4 import BeautifulSoup
import time
import json
import requests
import pandas as pd
from selenium.webdriver.support.wait import WebDriverWait
from tqdm import tqdm


# search_keys = ['yazılım mühendisi', 'iş analisti', 'pazarlama', 'satış', 'şoför']



def mine(search_keys):
    print("================ MINING FOR İSBULNET ================")
    print("\n")
    BASE_URL = 'https://isbul.net/is-ilanlari'
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("window-size=1920x1080")
    driver = webdriver.Chrome(chrome_options = chrome_options)
    jobs_list=[]

    for search_key in search_keys:
        driver.get(BASE_URL)
        time.sleep(0.4)
        driver.find_element_by_xpath("//input[@placeholder='ÖRN: İngilizce öğretmeni']").send_keys(search_key)
        time.sleep(0.5)
        #driver.find_element_by_xpath("//button[@class='c-button c-button--warning']").click()


        html_searched = driver.page_source
        pg_searched = BeautifulSoup(html_searched)

        NEW_URL =driver.current_url
        # print(NEW_URL)

        driver.close()
        driver = webdriver.Chrome(chrome_options = chrome_options)
        driver.get(NEW_URL)


        sourceText = driver.page_source
        startIndice = sourceText.find("var items =")
        startIndice+=11

        i = startIndice
        while sourceText[i]!='\n' :
            i+=1


        jsonText = sourceText[startIndice:i]
        jsonText= jsonText[:-1]
        #print(jsonText)
        jsonVal = json.loads(jsonText)
        i=0
        ilanSayisi=5
        ilanLinkleri = []
        while i<ilanSayisi:
            ilanLinkleri.append("https://isbul.net/is-ilani/"+jsonVal[i]['Slug'])
            print("https://isbul.net/is-ilani/"+jsonVal[i]['Slug'])
            i+=1

        i=0

        
        while i<ilanSayisi:
            job_dict = {}
            job_dict['url'] = ilanLinkleri[i]
            job_dict['keyword'] = search_key
            job_dict['src'] = 'isbulnet'
            driver.get(ilanLinkleri[i])
            time.sleep(0.2)
            try:
                html_job = driver.page_source
                pg_job = BeautifulSoup(html_job)
                job_title = driver.find_element_by_xpath('//div[@class="job-single-info3"]/h1').text
                job_dict['title']=job_title
                elements_temp = driver.find_elements_by_xpath('//div[@class="job-details"]/ul/li')
                job_dict['general-desc']=""
                for d_element in elements_temp:
                    job_dict['general-desc'] += d_element.text
                    job_dict['general-desc'] += ' '

                jobs_list.append(job_dict)
            except Exception as e:
                print(e)

            i+=1

    with open('jobs_isbulnet.json', 'w', encoding='utf8') as json_file:
        json.dump(jobs_list, json_file, ensure_ascii=False, indent = 4)
    driver.close()

    print("================ MINING COMPLETED ================")
