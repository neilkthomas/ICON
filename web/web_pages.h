/*
 * web_pages.h
 *
 *  Created on: Feb 5, 2022
 *      Author: Neil
 */



#ifndef WEB_WEB_PAGES_H_
#define WEB_WEB_PAGES_H_

const static char status_html[] ="<html xmlns=\"http://www.w3.org/1999/xhtml\"><head> <title>.: Icon :.</title> <style type=\"text/css\"> table{width: 773px;text-align:center;font-family:Tahoma;margin-top:40px;}div{text-align:center;}</style></head><body onload=\"startReqeust();\"> <div style=\"height: 551px\"><div><h3>Input Status</h3></div><table align=\"center\"> <thead><tr><th bgcolor=#ebeff0 >Channel</th><th bgcolor=#ebeff0 >Name</th><th bgcolor=#ebeff0 >Status</th><th bgcolor=#ebeff0 >Unit</th><th bgcolor=#ebeff0 >Type</th></tr></thead> <tbody id=\"ipstat\"></tbody> </table><div><h3>Output Status</h3></div><table align=\"center\"> <thead><tr><th bgcolor=#ebeff0 >Channel</th><th bgcolor=#ebeff0 >Name</th><th bgcolor=#ebeff0 >Status</th><th bgcolor=#ebeff0 >Unit</th> <th bgcolor=#ebeff0 >Type</th></tr></thead> <tbody id=\"opstat\"></tbody> </table><div><h3>Relay Status</h3></div><table align=\"center\"><thead><tr><th bgcolor=#ebeff0 >Channel</th><th bgcolor=#ebeff0 >Name</th><th bgcolor=#ebeff0 >Status</th></tr></thead> <tbody id=\"relstat\"></tbody> </table><div><h3>Counter Status</h3></div><table align=\"center\"><thead><tr><th bgcolor=#ebeff0 >Name</th><th bgcolor=#ebeff0 >Status</th></tr></thead> <tbody id=\"ctStat\"></tbody> </table><div><h3>Timer Status</h3></div><table align=\"center\"><thead><tr><th bgcolor=#ebeff0 >Name</th><th bgcolor=#ebeff0 >Status</th></tr></thead> <tbody id=\"tmrStat\"></tbody> </table></div></body><script type=\"text/javascript\">function startReqeust(){var t;window.XMLHttpRequest&&(t=new XMLHttpRequest),t.onreadystatechange=function(){t.readyState==XMLHttpRequest.DONE&&200==t.status&&hRes(t.responseText)};var e=\"http://\"+document.domain+\"/icon_status.cgi?sid=\"+Math.random();t.open(\"GET\",e,!0),t.send()}function hRes(t){var e=[],s=[],a=[],n=\"\";try{e=t.split(\"^\");for(var r=0;r<e.length;r++){n=\"\",s=e[r].split(\"|\");for(var o=0;o<s.length;o++){a=s[o].split(\",\"),n+=\"<tr>\";for(var i=0;i<a.length;i++)n+=0==i?\"<th>\"+a[i]+\"</th>\":\"<td>\"+a[i]+\"</td>\";n+=\"</tr>\"}0==r?set(\"ipstat\",n):1==r?set(\"opstat\",n):2==r?set(\"relstat\",n):3==r?set(\"ctStat\",n):4==r?set(\"tmrStat\",n):5==r?set(\"evStat\",n):6==r?set(\"stpStat\",n):7==r?set(\"schStat\",n):8==r?set(\"almStat\",n):9==r?set(\"swtStat\",n):10==r&&set(\"pgmStat\",n)}}catch(c){}setTimeout(hRes,1e3)}function set(t,e){document.getElementById(t).innerHTML=e}</script></html>";


#endif /* WEB_WEB_PAGES_H_ */


