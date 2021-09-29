

import sys
sys.path += [
    r'/home/ciaran/roadrunner/roadrunner/cmake-build-debug-wsl/lib/site-packages',
    # r'D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages'
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    # r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"
]



import roadrunner
from roadrunner._roadrunner import NamedArray
from roadrunner import Logger
print(roadrunner.__file__)
#
#
# n = NamedArray((3, 4))
# print(n)
# # print(n.__array_finalize__())
# n.colnames = [f'c{i}' for i in range(4)]
# n.rownames = [f'r{i}' for i in range(3)]
# print(n)



# binary = pickle.dumps(data)
#
# print(pickletools.dis(pickle.dumps(binary, protocol=5)))
# print(pickle.loads(binary))
# data2 = pickle.loads(binary)
# # print(type(data2))
# print(data2)
# sbml = """<?xml version="1.0" encoding="UTF-8"?>
# <sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
#   <model metaid="x" id="x">
#     <listOfCompartments>
#       <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
#     </listOfCompartments>
#     <listOfSpecies>
#       <species id="S1" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
#       <species id="S2" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
#     </listOfSpecies>
#     <listOfParameters>
#       <parameter id="kin" value="1" constant="true"/>
#       <parameter id="kf" value="0.1" constant="true"/>
#       <parameter id="kout" value="0.2" constant="true"/>
#       <parameter id="kb" value="0.01" constant="true"/>
#     </listOfParameters>
#     <listOfReactions>
#       <reaction id="_J0" reversible="false" fast="false">
#         <listOfProducts>
#           <speciesReference species="S1" stoichiometry="1" constant="true"/>
#         </listOfProducts>
#         <kineticLaw>
#           <math xmlns="http://www.w3.org/1998/Math/MathML">
#             <ci> kin </ci>
#           </math>
#         </kineticLaw>
#       </reaction>
#       <reaction id="_J1" reversible="false" fast="false">
#         <listOfReactants>
#           <speciesReference species="S1" stoichiometry="1" constant="true"/>
#         </listOfReactants>
#         <listOfProducts>
#           <speciesReference species="S2" stoichiometry="1" constant="true"/>
#         </listOfProducts>
#         <kineticLaw>
#           <math xmlns="http://www.w3.org/1998/Math/MathML">
#             <apply>
#               <times/>
#               <ci> kf </ci>
#               <ci> S1 </ci>
#             </apply>
#           </math>
#         </kineticLaw>
#       </reaction>
#       <reaction id="_J2" reversible="false" fast="false">
#         <listOfReactants>
#           <speciesReference species="S2" stoichiometry="1" constant="true"/>
#         </listOfReactants>
#         <kineticLaw>
#           <math xmlns="http://www.w3.org/1998/Math/MathML">
#             <apply>
#               <times/>
#               <ci> S2 </ci>
#               <ci> kout </ci>
#             </apply>
#           </math>
#         </kineticLaw>
#       </reaction>
#     </listOfReactions>
#   </model>
# </sbml>"""


n = NamedArray((2, 3))

print(sys.getrefcount(n))


