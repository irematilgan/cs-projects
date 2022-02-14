import selenium
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from bs4 import BeautifulSoup
import time
import requests
import pandas as pd
from tqdm import tqdm
import json

def applySearch(driver):
    # Arama Butonu
    driver.find_element_by_xpath("//button[@data-test='apply-button']").click()
    time.sleep(2)

def mine(search_keys, pg_length):
    print("================ MINING FOR KARİYERNET ================")
    print("\n")
    BASE_URL = 'https://www.kariyer.net/is-ilanlari'
    chrome_options = Options()
    chrome_options.add_argument("--headless")
    chrome_options.add_argument("window-size=1920x1080")
    driver = webdriver.Chrome(chrome_options = chrome_options)
    driver.set_page_load_timeout(30)
    driver.get(BASE_URL)
    driver.maximize_window()
    time.sleep(3)
    applySearch(driver)

    num_total_jobs = driver.find_element_by_xpath("//*[@class='t-6 text-secondary mb-3 search-result-section']").text
    print("TOPLAM IS = ", num_total_jobs)

    jobs_list = []
    for search_key in search_keys:
        html = driver.page_source
        pg = BeautifulSoup(html)
        time.sleep(3)
        # driver.find_element_by_class_name("vl-notif-delay").click()
        driver.find_element_by_xpath("//input[@data-test='searchInput']").send_keys(search_key) # anahtar kelimelerden biriyle arat

        driver.find_element_by_xpath("//button[@data-test='search-job-button']").click() # arat
        time.sleep(2)

        html_searched = driver.page_source
        pg_searched = BeautifulSoup(html)

        for page in range(pg_length):
            # jobs = driver.find_elements_by_class_name("list-items")
            jobs = driver.find_elements_by_xpath('//div[@class="list-items"]/a')
            print(jobs)
            # jobs = pg_searched.find_all("div",{"class" : "list-items"}) 
            # driver.find_element_by_class_name("list-items-wrapper")
            jobs_links = [job.get_attribute("href") for job in jobs]
            print(jobs_links)
            try:
                for job_link in jobs_links: # iş sayfalarını gez
                    job_dict = {}
                    job_dict['src'] = 'kariyernet'
                    job_dict['keyword'] = search_key
                    job_dict['url'] = job_link
                    # job_link = job.a['href']
                    driver.get(job_link)
                    driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
                    time.sleep(5)

                    html_job = driver.page_source
                    pg_job = BeautifulSoup(html)
                    
                    job_title = driver.find_element_by_id("jobTitle").text
                    job_dict['title'] = job_title # Unvani kaydet

                    spans = driver.find_elements_by_xpath('//*[@class="genel-nitelikler"]/ul/li/span')
                    # genel-nitelikler bolumu
                    job_dict['general-desc'] = []
                    for span in spans:
                        job_dict['general-desc'].append(span.text.split("&nbsp;")[-1])

                    # aday kriterleri bolumu
                    job_dict['candidate-criterias'] = []
                    cols = driver.find_elements_by_xpath('//*[@class="sub-box aday-kriterleri"]/div/div/div/p')
                    # rows = pg_job.find("div",{"class" : "sub-box aday-kriterleri"}).find_all("div",{"class" : "row"})
                    for i, col in enumerate(cols):
                        if i % 2 == 0:
                            continue
                        job_dict['candidate-criterias'].append(col.text)


                    #pozisyon bilgileri bolumu
                    job_dict['position-info'] = []
                    cols = driver.find_elements_by_xpath('//*[@class="sub-box pozisyon-bilgileri"]/div/div/div/p')
                    for i, col in enumerate(cols):
                        if i % 2 == 0:
                            continue
                        job_dict['position-info'].append(col.text)
                    
                    
                    job_dict['sector'] = driver.find_element_by_id("jobSector").text
                    # print(job_dict)
                    jobs_list.append(job_dict)
                    time.sleep(3)
                    driver.back()
                
                # Sonraki Sayfa
                driver.find_element_by_xpath('//button[@class="page-link"]').click()
            except Exception as e:
                print(e)

        with open('jobs_kariyernet.json', 'w', encoding='utf8') as json_file:
            json.dump(jobs_list, json_file, ensure_ascii=False, indent = 4)

        driver.get(BASE_URL)
        time.sleep(2)

            
    print(jobs_list)
    #json_jobs = json.dumps(jobs_list, indent = 4)
    with open('jobs_kariyernet.json', 'w', encoding='utf8') as json_file:
        json.dump(jobs_list, json_file, ensure_ascii=False, indent = 4)
    driver.close()

    print("================ MINING COMPLETED ================")