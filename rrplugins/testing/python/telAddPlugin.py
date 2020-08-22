import teplugins as tel

try:     
    p = tel.Plugin('tel_AddPlugin')
    p.x = 1.2
    p.y = 3.6
    print p.execute()

    print p.z
except Exception as e:
    print e     