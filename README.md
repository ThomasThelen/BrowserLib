# BrowserLib
A library which makes modification of Internet Explorer quick and easy.

##Motivation
Changing a web browser's behavior is a trivial, time consuming task which can be done via modifying relevant registry keys. It is common for malware to change said values to change search engines, home page, disable security, etc... This project is aimed at providing easy access to check if the properties have been changed, and if so, set them to a custom value.
<br>

##Features

  1. Ability to obtain
  <br>
    a. Current Version
  <br>
    b. Current Home Page
  <br>
    c. Current Path of Instillation
  <br>
    d. Current Search Engine
  <br>
    e. Check whether the phishing filter is enabled
  2. Ability to set
  <br>
    a. Home Page
  <br>
    b. Path of Instillation
  <br>
    c. Search Engine
  <br>
    d. Phishing Filter
  <br>
  3. Google Chrome Planned
  <br>
    a. Home Page
  <br>
    b. Search Engine

##Instructions
Include browserlib.h in your c++ file.
<br>
The IExplorer class is the main Internet Explorer interface. 
<br>
The SEARCHENGINE structure is passed to the SetSearchEngine function and must be filled beforehand. In it are all the parameters needed to create a new search engine registry entry.
<br>
##Examples
###InternetExplorer.cpp
This is an example on how to monitor certain Internet Explorer parameters. This can be extended to include 

###InternetExplorer2.cpp
This shows example usage of the IExplorer class.
