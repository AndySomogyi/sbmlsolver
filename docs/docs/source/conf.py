# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'libomexmeta'
copyright = '2020, Ciaran Welsh'
author = 'Ciaran Welsh'

# The full version, including alpha/beta/rc tags
release = '0.0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "breathe",
    # "exhale",
    # 'sphinx.ext.pngmath',
    'sphinx.ext.todo',
    "sphinxcontrib.bibtex",
    "sphinx_tabs.tabs",
    "sphinx.ext.doctest"
]

doctest_global_setup = '''
try:
    
    import os
    import site
    import unittest
    
    # take care of directories so we can test the pyomexmeta api
    docs_dir = os.path.abspath(os.path.dirname(__file__))
    proj_dir = os.path.dirname(docs_dir)
    src_dir = os.path.join(proj_dir, "src")
    pyomexmeta_dir = os.path.join(src_dir, "pyomexmeta")
    
    site.addsitedir(src_dir)
    
    from pyomexmeta import PyOmexMetaAPI
except ImportError as e:
    print("could not import pyomexmeta, is it installed? ")
    raise e
'''
# Breathe Configuration
breathe_default_project = "libomexmeta"
# "D:\libOmexMeta\docs\source\conf.py"
breathe_projects = {
    "libomexmeta": "../doxygen-output/xml/"
}

# # Setup the exhale extension
# exhale_args = {
#     # These arguments are required
#     "containmentFolder":     "./APIReference",
#     "rootFileName":          "api_reference_index.rst",
#     "rootFileTitle":         "libOmexMeta API Reference",
#     "doxygenStripFromPath":  "..",
#     # Suggested optional arguments
#     "createTreeView":        True,
#     # TIP: if using the sphinx-bootstrap-theme, you need
#     # "treeViewIsBootstrap": True,
#     "exhaleExecutesDoxygen": False,
#     "exhaleDoxygenStdin":    "INPUT = ../../src/omexmeta"
# }
#
# # Tell sphinx what the primary language being documented is.
# primary_domain = 'cpp'
#
# # Tell sphinx what the pygments highlight language should be.
# highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']