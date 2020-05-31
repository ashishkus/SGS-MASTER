# SGS-MASTER
The purpose of this project is to make power grid system more efficient and power saving. And saving our environment as less losses occur.

it includes 4 techniques:-

1.Load Distribution:Using relays,Arduino and some code , we switched the excess load of power of one sub-transformer to another sub-transformer after sensing the current

2.Appliance Ranking:We used current sensors SCT013 to sense Current of different company's appliances and send it to NodeMCU which is a microcontroller with Wifi chip.These current values are in bits so they are converted to rms current and send to ThingsSpeak cloud APIs.Graph is drawn in ThingsSpeak.We wrote python code which would take values from ThingsSpeak using Read API key and form a database using SQLite.This database would be the ranking of an appliance of different companies according to their efficiency. Advantage:Consumers will know the company with most efficient appliance

3.Detection of position of Transmission Losses: In a smart grid , there are many transformers , sub-transformer situated at different places.There should be no power consumption between transformer and sub-transformer as there is no load but if even though power is consumed means at that location power loss is occuring .So, for this the wifi chip will send the power values of different nodes to cloud and there if there is any difference of power between two nodes then an event is triggered to IFTTT and IFTTT sends mail to power authority about the loss.For monitoring of values , we did MATLAB analysis. Upload the switchgear.ino on Arduino for Load distribution Upload the grid.ino on NodeMCU for other techniques.
