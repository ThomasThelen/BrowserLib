# BrowserLib
A library which makes modification of Internet Explorer quick and easy.

## Motivation
Many of a web bowers' properties are stored in the registry. It is common for malware to change these values to route victims to custom home pages, search engines, etc... 

This library provides access to 

## Features

  1. Ability to obtain


    a. Current Version


    b. Current Home Page


    c. Current Path of Instillation


    d. Current Search Engine


    e. Check whether the phishing filter is enabled
  2. Ability to set


    a. Home Page


    b. Path of Instillation


    c. Search Engine


    d. Phishing Filter


  3. Google Chrome Planned


    a. Home Page


    b. Search Engine

## Instructions

Include browserlib.h in your c++ file.


The IExplorer class is the main Internet Explorer interface. 


The SEARCHENGINE structure is passed to the SetSearchEngine function and must be filled beforehand. In it are all the parameters needed to create a new search engine registry entry.


## Examples

### InternetExplorer.cpp

This is an example on how to monitor certain Internet Explorer parameters. This can be extended to run in the background to protect against unknown modifications.


### InternetExplorer2.cpp

This shows example usage of the IExplorer class and its methdos.

