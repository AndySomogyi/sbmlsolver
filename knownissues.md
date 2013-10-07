---
layout: page
title: "Known Issues"
description: ""
---
{% include JB/setup %}

#RoadRunner++ Version 1.0
##Visual Studio Build Issues
When building the ThirdParty libraries, the release build flags Ob2 and O2 need to be changed to Ob0 and Od respectively. This is a problem with Sundials CVODE solver. If you do not change these flags, you will get a "Error in CVODE: -4" error when trying to simulate.

##libSBML Test Suite Failing Tests
The table below shows tests that should be passing, but for various reasons fails. Tests beyond case 980 was not checked

|Count |Case Number |Problem Description                            |
|------|------------|-----------------------------------------------|
|1     |950         |Handling of infinity constants, +/-INF and NAN |
|2     |951         |Handling of infinity constants, +/-INF and NAN |
|3     |955         |Various mathematical constructs                |
|4     |957         |Various mathematical constructs                |
|5     |958         |Various mathematical constructs                |
|6     |959         |Various mathematical constructs                |
|7     |961         |Avogadros constant test                        |
|8     |962         |Random testing of events                       |
|9     |978         |Some event triggering (12 events)              |

##Recently Fixed Tests
The following cases were recently 'fixed'

|Case Number |Problem Description               |
|------------|----------------------------------|
|935	     |Proper event not executed	        |
|952	     |Event stuff including 'persistent'|
|963	     |Event triggering                  |
|964	     |Random triggering                 |
|965	     |Similar too 952                   |
|966	     |Similar too 952                   |
|967	     |Random triggering                 |

##Not supported cases
In addition to the above failing cases, the following (83) cases are failing as well, but doing so because of not being currently supported:

39, 40,182,184,531,533,534,535,536,537,538,543,546,548,549,


550,551,552,553,554,555,556,557,558,559,560,561,562,563,564, 565,566,567,569,570,571,573,575,576,613,614,615,628,629,630, 658,659,660,661,662,663,664,665,666,673,674,675,687,695,696, 705,760,761,762,777,778,779,780,844,870,871,872,873,874,875, 876,937,938,939,940,941,942,943
