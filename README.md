# Recommender-System
Recommender System
This project is a tool that enables people to find "people like them" according to some simple criteria.

Part 1

In the first step I build an in memory dataset. On startup, the program creates an in memory datastore about people where each record includes name, latitude, longitude and age. Other fields can be added (in the code). The data is, but realistic. It doesn't have an accurate statistical distribution or anything like that, but records do look like real people. The datastore contain between 10 million and 100 million entries (defined in the code).

Part 2

Once the datastore is populated, an interface to query the datastore is prompted. This is a command-line interface. It allows the user to enter their age, latitude and longitude, and provides a list of 10 people who are "similar" -- i.e., of a similar age, and living nearby. It provides the 10 closest people within 1 second for at least 95% of queries. The results show the name, age and location of each of the 10 recommended people.

