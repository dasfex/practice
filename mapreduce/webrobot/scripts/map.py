#! /usr/bin/python3

from bs4 import BeautifulSoup
from urllib.request import Request, urlopen
import sys

for line in sys.stdin:
    args = list(line.split('\t'))
    if len(args) == 0:
        continue
    
    address = args[0]
    print(address, '\t', '+')
    if len(args) == 1 or (len(args) == 2 and args[1][-1] == '\n'):
        html_page = ''
        try:
            hdr = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) '
                                 'AppleWebKit/537.36 (KHTML, like Gecko) '
                                 'Chrome/39.0.2171.71 Safari/537.36'}
            request = Request(address, headers=hdr)
            html_page = urlopen(request)
            soup = BeautifulSoup(html_page, "html.parser", from_encoding="iso-8859-1")

            new_links = set()
            for link in soup.findAll('a'):
                if link.get('href') != None and \
                        link.get('href') != '#' and \
                        link.get('href')[0:5] == 'https':
                    new_link = link.get('href')
                    if new_link[-1] == '/':
                        new_link = new_link[:-1]
                    new_links.add(new_link)
        except:
            print('http error 999 request denied at link: ', args[0], file=sys.stderr)

        for link in new_links:
            print(link, '\t')
