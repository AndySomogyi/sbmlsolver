"""
Created on Jun 23, 2014

@author: Andy Somogyi

SBML test suite functionality


"""

import roadrunner as rr
import numpy as npy
import os.path as path
import os
import glob


class SBMLTest(object):
    """
    Run an SBML test suite test.

    The constructor takes a single string of the test directory name. The test can then 
    be run using the `run()` method, i.e. ::
        
        test = SBMLTest("/path/to/sbml_tests/00001")
        ok = test.run()

        if ok:
            print("test " + test.test + " passed")
        else:
            print("test " + test.test + " failed, errors: ")
            print(test.errors)
    """


    def __init__(self, dirName):
        
        if not path.isdir(dirName):
            raise Exception(dirName + " is not a directory")

        self.testDir = dirName = path.abspath(dirName)

        self.test = path.basename(dirName)

        # get the settings file
        settingsName = path.join(dirName, path.basename(dirName) + "-settings.txt")
        if not path.isfile(settingsName):
            raise Exception(dirName + " does not have a settings file of " + settingsName)
        
        self.settingspath = settingsName
        
        opt = rr.SimulateOptions()
        opt.loadSBMLSettings(settingsName)

        # origininal copy of options.
        self.settings = opt.copy()

        # find the best source file
        # the candidate file names
        fileNames = [path.join(dirName, path.basename(dirName) + suffix) for suffix in
                     ["-sbml-l3v1.xml", "-sbml-l2v4.xml", "-sbml-l2v3.xml", 
                      "-sbml-l2v2.xml", "-sbml-l2v1.xml", "-sbml-l1v2.xml"]]

        # remove the ones that are not files
        fileNames = [f for f in fileNames if path.isfile(f)]
        
        if len(fileNames) < 1:
            raise Exception("could not file sbml test file in directory " + dirName)
        
        # the best file
        self.sbmlFileName = fileNames[0]

        # get the results file
        resultsName = path.join(dirName, path.basename(dirName) + "-results.csv")
        self.expectedResult = npy.genfromtxt(resultsName, delimiter=',',  dtype=npy.float64, names=True)

        # its a structured array, column names in csv.dtype.names

        self.names = self.expectedResult.dtype.names

        # no result yet
        self.result = None

        # no errors yet
        self.errors = []



    def getDifferences(self):
        """
        gets the absolute differences between the expected results and 
        the simulation results as a regular numpy array
        """

        if self.result is None:
            raise Exception("simulation has not been run yet")

        diff = self.expectedResult.copy()
        
        # check have the same colums and points
        resultNames = list(self.result.dtype.names)
        
        for name in self.names:
            # have different names in test data and rr results, we add a [S1] brackets
            # to concentrations.

            resName = name

            # check amount names
            if not name in resultNames:
                # maybe its a concentrion:
                resName = "[" + name + "]"
                if not resName in resultNames:
                    raise Exception("result missing expected column '" + name + "'")

            diff[name] = npy.abs(self.expectedResult[name] - self.result[resName])

        return diff.view((npy.float64, len(diff.dtype.names)))


    def getTestResults(self, r, diff=None):
        if diff is None:
            diff = self.getDifferences()

        result = self.result.view((npy.float64, len(self.result.dtype.names)))

        return diff <= (r.integrator.absolute_tolerance + (r.integrator.relative_tolerance * npy.abs(result)))        

    def run(self):
        """
        Run the SBML test. Returns True or False depending on if the test passes or not. 
        If the test fails, any errors will be available in the SBMLTest.errors property.
        """
        
        self.errors = []
        result = True
        
        try:
            # run the simulation

            r = rr.RoadRunner(self.sbmlFileName, _getLoadOptions())
            
            # load integrator settings
            r.integrator.loadSBMLSettings(self.settingspath)

            # need to tweak the tolerances for the current integrator
            opt = self.settings.copy()
            #opt.tweakTolerances()
            opt.structuredResult = True
            self.result = r.simulate(opt)

            # logical array of pass / fail test points.
            test = self.getTestResults(r)

            for i in npy.arange(len(self.names)):
                ok = npy.all(test[:,i])
                if not ok:
                    err = "'" + self.names[i] + "' failed " + str(test.shape[0] - npy.count_nonzero(test[:,i])) + \
                          " out of " + str(test.shape[0]) + " points."
                    self._addError(err)
                    result = False

        except Exception as e:
            result = False
            self._addError(str(e))

        return result

    def diffplot(self):
        """
        Run the SBML test. Returns True or False depending on if the test passes or not. 
        If the test fails, any errors will be available in the SBMLTest.errors property.
        """

        import matplotlib.pyplot as plt
        
        self.errors = []
        result = True
        
        try:
            # run the simulation

            plt.figure(1)

            r = rr.RoadRunner(self.sbmlFileName, _getLoadOptions())

            # need to tweak the tolerances for the current integrator
            opt = self.settings.copy()
            opt.tweakTolerances()
            opt.structuredResult = True

            plt.subplot(211)
            r.simulate(opt, plot=True, show=False)
            plt.xlim([0, opt.duration])


            r = rr.RoadRunner(self.sbmlFileName, _getLoadOptions())

            # need to tweak the tolerances for the current integrator
            opt = self.settings.copy()
            opt.tweakTolerances()
            opt.variableStep = True
            opt.structuredResult = True

            plt.subplot(212)
            r.simulate(opt, plot=True, show=False)
            plt.xlim([0, opt.duration])


            plt.show()


        except Exception as e:
            result = False
            self._addError(str(e))

        return result


    def _addError(self, err):
        self.errors += [err]


def _getLoadOptions():
    """
    Performance is heavily depdent on the options given, this set of options generates
    essentially generates a model without mutable init conditions and other costly 
    features which are not used in pure time series simulations.
    """
    
    opt = rr.LoadSBMLOptions()
    
    # don't generate cache for models
    opt.modelGeneratorOpt |= rr.LoadSBMLOptions.RECOMPILE
    
    # no mutable initial conditions
    opt.modelGeneratorOpt &= ~rr.LoadSBMLOptions.MUTABLE_INITIAL_CONDITIONS
    
    # read only model
    opt.modelGeneratorOpt |= rr.LoadSBMLOptions.READ_ONLY
    
    opt.modelGeneratorOpt |= rr.LoadSBMLOptions.OPTIMIZE_CFG_SIMPLIFICATION
    
    opt.modelGeneratorOpt |= rr.LoadSBMLOptions.OPTIMIZE_GVN
    
    return opt


def runSBMLTests(sbmlTestDir):
    """
    Run the sbml tests in the given test directory, 
    this uses the list of tests from `getSupportedSBMLTests()`.
    """
    import time
    if not path.isdir(sbmlTestDir):
        raise Exception(sbmlTestDir + " does not appear to exist")

    sbmlTestDir = path.abspath(sbmlTestDir)

    cases = getSupportedSBMLTests()

    start = time.clock() 
    for case in cases:
        runSBMLTest(sbmlTestDir, case)
    
    elapsed = time.clock()
    elapsed = elapsed - start
    print("Completed " + str(len(cases)) + " tests in " + str(elapsed) + " seconds.")

    
def runSBMLTest(sbmlTestDir, case):
    """
    Run a single sbml test case. 

    :param sbmlTestDir: the directory where the sbml tests are located, this would
    be the directory where you unziped the sbml test suite to.

    :param case: the sbml test case. This may be a number, or it may be a 
    text string to use as the exact test case directory, i.e. 1, or "00001".
    """

    import roadrunner as rr

    # if case is a number, we convert to the correct textual form
    # padd the with the correct number of zeros.
    if type(case) == int:
        case = str(case).zfill(5)

    rr.Logger.log(rr.Logger.LOG_NOTICE, "running test " + case)

    # the test dir may have /cases/semantic/ sub directories
    if not path.isdir(path.join(sbmlTestDir, case)) and \
        path.isdir(path.join(sbmlTestDir, "cases")):
            sbmlTestDir = path.join(sbmlTestDir, "cases")

            if not path.isdir(path.join(sbmlTestDir, case)) and \
                path.isdir(path.join(sbmlTestDir, "semantic")):
                    sbmlTestDir = path.join(sbmlTestDir, "semantic")

    test = SBMLTest(path.join(sbmlTestDir, case))
    ok = test.run()

    if ok:
        rr.Logger.log(rr.Logger.LOG_NOTICE, "test " + case + " passed")
    else:
        rr.Logger.log(rr.Logger.LOG_ERROR, "test " + case + " failed, errors: ")
        for e in test.errors:
            rr.Logger.log(rr.Logger.LOG_ERROR, e)


def plotSBMLTest(sbmlTestDir, case):
    """
    Run a single sbml test case. 

    :param sbmlTestDir: the directory where the sbml tests are located, this would
    be the directory where you unziped the sbml test suite to.

    :param case: the sbml test case. This may be a number, or it may be a 
    text string to use as the exact test case directory, i.e. 1, or "00001".
    """

    import roadrunner as rr

    # if case is a number, we convert to the correct textual form
    # padd the with the correct number of zeros.
    if type(case) == int:
        case = str(case).zfill(5)

    rr.Logger.log(rr.Logger.LOG_NOTICE, "running test " + case)

    # the test dir may have /cases/semantic/ sub directories
    if not path.isdir(path.join(sbmlTestDir, case)) and \
        path.isdir(path.join(sbmlTestDir, "cases")):
            sbmlTestDir = path.join(sbmlTestDir, "cases")

            if not path.isdir(path.join(sbmlTestDir, case)) and \
                path.isdir(path.join(sbmlTestDir, "semantic")):
                    sbmlTestDir = path.join(sbmlTestDir, "semantic")

    test = SBMLTest(path.join(sbmlTestDir, case))
    test.diffplot()





def getSupportedSBMLTests():
    """
    get a list of the support RoadRunner SBML tests cases
    """
        
    tests = [
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "11",
        "12",
        "13",
        "14",
        "15",
        "16",
        "17",
        "18",
        "19",
        "20",
        "21",
        "22",
        "23",
        "24",
        "25",
        "26",
        "27",
        "28",
        "29",
        "30",
        "31",
        "32",
        "33",
        "34",
        "35",
        "36",
        "37",
        "38",
        "41",
        "42",
        "43",
        "44",
        "45",
        "46",
        "47",
        "48",
        "49",
        "50",
        "51",
        "52",
        "53",
        "54",
        "55",
        "56",
        "57",
        "58",
        "59",
        "60",
        "61",
        "62",
        "63",
        "64",
        "65",
        "66",
        "67",
        "68",
        "69",
        "70",
        "71",
        "72",
        "73",
        "74",
        "75",
        "76",
        "77",
        "78",
        "79",
        "80",
        "81",
        "82",
        "83",
        "84",
        "85",
        "86",
        "87",
        "88",
        "89",
        "90",
        "91",
        "92",
        "93",
        "94",
        "95",
        "96",
        "97",
        "98",
        "99",
        "100",
        "101",
        "102",
        "103",
        "104",
        "105",
        "106",
        "107",
        "108",
        "109",
        "110",
        "111",
        "112",
        "113",
        "114",
        "115",
        "116",
        "117",
        "118",
        "119",
        "120",
        "121",
        "122",
        "123",
        "124",
        "125",
        "126",
        "127",
        "128",
        "129",
        "130",
        "131",
        "132",
        "133",
        "134",
        "135",
        "136",
        "137",
        "138",
        "139",
        "140",
        "141",
        "142",
        "143",
        "144",
        "145",
        "146",
        "147",
        "148",
        "149",
        "150",
        "151",
        "152",
        "153",
        "154",
        "155",
        "156",
        "157",
        "158",
        "159",
        "160",
        "161",
        "162",
        "163",
        "164",
        "165",
        "166",
        "167",
        "168",
        "169",
        "170",
        "171",
        "172",
        "173",
        "174",
        "175",
        "176",
        "177",
        "178",
        "179",
        "180",
        "181",
        "183",
        "185",
        "186",
        "187",
        "188",
        "189",
        "190",
        "191",
        "192",
        "193",
        "194",
        "195",
        "196",
        "197",
        "198",
        "199",
        "200",
        "201",
        "202",
        "203",
        "204",
        "205",
        "206",
        "207",
        "208",
        "209",
        "210",
        "211",
        "212",
        "213",
        "214",
        "215",
        "216",
        "217",
        "218",
        "219",
        "220",
        "221",
        "222",
        "223",
        "224",
        "225",
        "226",
        "227",
        "228",
        "229",
        "230",
        "231",
        "232",
        "233",
        "234",
        "235",
        "236",
        "237",
        "238",
        "239",
        "240",
        "241",
        "242",
        "243",
        "244",
        "245",
        "246",
        "247",
        "248",
        "249",
        "250",
        "251",
        "252",
        "253",
        "254",
        "255",
        "256",
        "257",
        "258",
        "259",
        "260",
        "261",
        "262",
        "263",
        "264",
        "265",
        "266",
        "267",
        "268",
        "269",
        "270",
        "271",
        "272",
        "273",
        "274",
        "275",
        "276",
        "277",
        "278",
        "279",
        "280",
        "281",
        "282",
        "283",
        "284",
        "285",
        "286",
        "287",
        "288",
        "289",
        "290",
        "291",
        "292",
        "293",
        "294",
        "295",
        "296",
        "297",
        "298",
        "299",
        "300",
        "301",
        "302",
        "303",
        "304",
        "305",
        "306",
        "307",
        "308",
        "309",
        "310",
        "311",
        "312",
        "313",
        "314",
        "315",
        "316",
        "317",
        "318",
        "319",
        "320",
        "321",
        "322",
        "323",
        "324",
        "325",
        "326",
        "327",
        "328",
        "329",
        "330",
        "331",
        "332",
        "333",
        "334",
        "335",
        "336",
        "337",
        "338",
        "339",
        "340",
        "341",
        "342",
        "343",
        "344",
        "345",
        "346",
        "347",
        "348",
        "349",
        "350",
        "351",
        "352",
        "353",
        "354",
        "355",
        "356",
        "357",
        "358",
        "359",
        "360",
        "361",
        "362",
        "363",
        "364",
        "365",
        "366",
        "367",
        "368",
        "369",
        "370",
        "371",
        "372",
        "373",
        "374",
        "375",
        "376",
        "377",
        "378",
        "379",
        "380",
        "381",
        "382",
        "383",
        "384",
        "385",
        "386",
        "387",
        "388",
        "389",
        "390",
        "391",
        "392",
        "393",
        "394",
        "395",
        "396",
        "397",
        "398",
        "399",
        "400",
        "401",
        "402",
        "403",
        "404",
        "405",
        "406",
        "407",
        "408",
        "409",
        "410",
        "411",
        "412",
        "413",
        "414",
        "415",
        "416",
        "417",
        "418",
        "419",
        "420",
        "421",
        "422",
        "423",
        "424",
        "425",
        "426",
        "427",
        "428",
        "429",
        "430",
        "431",
        "432",
        "433",
        "434",
        "435",
        "436",
        "437",
        "438",
        "439",
        "440",
        "441",
        "442",
        "443",
        "444",
        "445",
        "446",
        "447",
        "448",
        "449",
        "450",
        "451",
        "452",
        "453",
        "454",
        "455",
        "456",
        "457",
        "458",
        "459",
        "460",
        "461",
        "462",
        "463",
        "464",
        "465",
        "466",
        "467",
        "468",
        "469",
        "470",
        "471",
        "472",
        "473",
        "474",
        "475",
        "476",
        "477",
        "478",
        "479",
        "480",
        "481",
        "482",
        "483",
        "484",
        "485",
        "486",
        "487",
        "488",
        "489",
        "490",
        "491",
        "492",
        "493",
        "494",
        "495",
        "496",
        "497",
        "498",
        "499",
        "500",
        "501",
        "502",
        "503",
        "504",
        "505",
        "506",
        "507",
        "508",
        "509",
        "510",
        "511",
        "512",
        "513",
        "514",
        "515",
        "516",
        "517",
        "518",
        "519",
        "520",
        "521",
        "522",
        "523",
        "524",
        "525",
        "526",
        "527",
        "528",
        "529",
        "530",
        "532",
        "539",
        "540",
        "541",
        "542",
        "544",
        "545",
        "547",
        "568",
        "572",
        "574",
        "577",
        "578",
        "579",
        "580",
        "581",
        "582",
        "583",
        "584",
        "585",
        "586",
        "587",
        "588",
        "589",
        "590",
        "591",
        "592",
        "593",
        "594",
        "595",
        "596",
        "597",
        "598",
        "599",
        "600",
        "601",
        "602",
        "603",
        "604",
        "605",
        "606",
        "607",
        "608",
        "609",
        "610",
        "611",
        "612",
        "616",
        "617",
        "618",
        "619",
        "620",
        "621",
        "622",
        "623",
        "624",
        "625",
        "626",
        "627",
        "631",
        "632",
        "633",
        "634",
        "635",
        "636",
        "637",
        "638",
        "639",
        "640",
        "641",
        "642",
        "643",
        "644",
        "645",
        "646",
        "647",
        "648",
        "649",
        "650",
        "651",
        "652",
        "653",
        "654",
        "655",
        "656",
        "657",
        "667",
        "668",
        "669",
        "670",
        "671",
        "672",
        "676",
        "677",
        "678",
        "679",
        "680",
        "681",
        "682",
        "683",
        "684",
        "685",
        "686",
        "688",
        "689",
        "690",
        "691",
        "692",
        "693",
        "694",
        "697",
        "698",
        "699",
        "700",
        "701",
        "702",
        "703",
        "704",
        "706",
        "707",
        "708",
        "709",
        "710",
        "711",
        "712",
        "713",
        "714",
        "715",
        "716",
        "717",
        "718",
        "719",
        "720",
        "721",
        "722",
        "723",
        "724",
        "725",
        "726",
        "727",
        "728",
        "729",
        "730",
        "731",
        "732",
        "733",
        "734",
        "735",
        "736",
        "737",
        "738",
        "739",
        "740",
        "741",
        "742",
        "743",
        "744",
        "745",
        "746",
        "747",
        "748",
        "749",
        "750",
        "751",
        "752",
        "753",
        "754",
        "755",
        "756",
        "757",
        "758",
        "759",
        "763",
        "764",
        "765",
        "766",
        "767",
        "768",
        "769",
        "770",
        "771",
        "772",
        "773",
        "774",
        "775",
        "776",
        "781",
        "782",
        "783",
        "784",
        "785",
        "786",
        "787",
        "788",
        "789",
        "790",
        "791",
        "792",
        "793",
        "794",
        "795",
        "796",
        "797",
        "798",
        "799",
        "800",
        "801",
        "802",
        "803",
        "804",
        "805",
        "806",
        "807",
        "808",
        "809",
        "810",
        "811",
        "812",
        "813",
        "814",
        "815",
        "816",
        "817",
        "818",
        "819",
        "820",
        "821",
        "822",
        "823",
        "824",
        "825",
        "826",
        "827",
        "828",
        "829",
        "830",
        "831",
        "832",
        "833",
        "834",
        "835",
        "836",
        "837",
        "838",
        "839",
        "840",
        "841",
        "842",
        "843",
        "845",
        "846",
        "847",
        "848",
        "849",
        "850",
        "851",
        "852",
        "853",
        "854",
        "855",
        "856",
        "857",
        "858",
        "859",
        "860",
        "861",
        "862",
        "863",
        "864",
        "865",
        "866",
        "867",
        "868",
        "869",
        "877",
        "878",
        "879",
        "880",
        "881",
        "882",
        "883",
        "884",
        "885",
        "886",
        "887",
        "888",
        "889",
        "890",
        "891",
        "892",
        "893",
        "894",
        "895",
        "896",
        "897",
        "898",
        "899",
        "900",
        "901",
        "902",
        "903",
        "904",
        "905",
        "906",
        "907",
        "908",
        "909",
        "910",
        "911",
        "912",
        "913",
        "914",
        "915",
        "916",
        "917",
        "918",
        "919",
        "920",
        "921",
        "922",
        "923",
        "924",
        "925",
        "926",
        "927",
        "928",
        "929",
        "930",
        "931",
        "932",
        "934",
        "935",
        "936",
        "944",
        "945",
        "946",
        "947",
        "948",
        "949",
        "952",
        "953",
        "954",
        "955",
        "956",
        "957",
        "958",
        "960",
        "961",
        "962",
        "963",
        "964",
        "965",
        "966",
        "967",
        "968",
        "969",
        "970",
        "971",
        "972",
        "973",
        "974",
        "975",
        "976",
        "977",
        "978",
        "979",
        "980",
        "989",
        "990",
        "991",
        "992",
        "994",
        "995",
        "996",
        "997",
        "998",
        "999",
        "1001",
        "1002",
        "1003",
        "1004",
        "1005",
        "1006",
        "1007",
        "1008",
        "1009",
        "1010",
        "1011",
        "1012",
        "1013",
        "1014",
        "1015",
        "1016",
        "1017",
        "1018",
        "1019",
        "1020",
        "1021",
        "1022",
        "1023",
        "1024",
        "1025",
        "1026",
        "1027",
        "1028",
        "1029",
        "1030",
        "1031",
        "1032",
        "1033",
        "1034",
        "1035",
        "1036",
        "1037",
        "1038",
        "1039",
        "1040",
        "1041",
        "1042",
        "1043",
        "1045",
        "1046",
        "1047",
        "1048",
        "1049",
        "1050",
        "1055",
        "1056",
        "1057",
        "1058",
        "1059",
        "1060",
        "1061",
        "1062",
        "1063",
        "1064",
        "1065",
        "1066",
        "1067",
        "1068",
        "1069",
        "1070",
        "1071",
        "1072",
        "1073",
        "1074",
        "1075",
        "1076",
        "1077",
        "1078",
        "1079",
        "1080",
        "1081",
        "1082",
        "1087",
        "1088",
        "1089",
        "1090",
        "1091",
        "1092",
        "1093",
        "1094",
        "1095",
        "1096",
        "1097",
        "1098",
        "1099",
        "1100",
        "1101",
        "1102",
        "1103",
        "1104",
        "1105",
        "1106",
        "1107",
        "1109",
        "1110",
        "1111",
        "1116",
        "1117",
        "1124",
        "1125",
        "1149",
        "1150",
        "1151",
        "1162",
        "1163",
        "1166",
        "1184",
        "1185"]


    return [test.zfill(5) for test in tests]
 

        
        
        
