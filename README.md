# SAM Sensorauswertung

SAM stands for Sensors And More. <br> <br>
This Software does the following:<br>
1. Read a NMEA 0183 Recommended Minimum Sentence C (RMC) plus a following Sensorvalue. <br>
2. Store the data of each sentence (date, time, position, sensorvalue etc.) into a dataclass. <br>
3. Scan trough the container with datas and check for outliers and remove them. <br>
4. Convert data to the .czml format to use them in the cesium viewer (https://sandcastle.cesium.com/?src=CZML.html&label=DataSources) <br>
5. Show a GUI, in which the cesium viewer webpage get called and a .czml file can be loaded by drag and drop. Positions and values will be shown as colored dots on the map. 
Additional infos for the sensorvalues will be displayed trough a bar graph. <br><br>

This software was developed by Rick Fastenrath and Erik Falk during winter semester 2015/2016 at FH Kiel (www.fh-kiel.de). <br> <br>
Used tools and libs: <br>
Qt 5.5 <br>
CesiumViewer and the czml fileformat <br>
QCustomPlot <br>
ALGLIB <br>




