from bioservices import BioModels

s = BioModels()

models = s.get_all_models()

print(dir(BioModels))