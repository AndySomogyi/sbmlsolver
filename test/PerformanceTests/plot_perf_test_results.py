import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np
import os
sns.set_context("paper", font_scale=1.5)

df = pd.read_csv("RoadRunnerMapPerformanceTestsWithVTuneThreadingAnalysisResults.csv")

print(df.columns)
df = df.set_index(['Jit Compiler', 'Number of threads'])
print(df)

fig = plt.figure()
sns.barplot(
    data=df.reset_index(), y="Elapsed Time (s)", x="Number of threads", hue="Jit Compiler",
    linewidth=1.0, edgecolor="black"
)
sns.despine(fig=fig)
plt.title("Time taken for RoadRunnerMap to load first 100 models\n from the sbml test suite.")

fname = os.path.join(os.path.dirname(__file__), "RoadRunnerMapPerfTestWithVTune.png")
plt.savefig(fname, dpi=250, bbox_inches="tight")
















