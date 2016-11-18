import os
import sys

import sphinx_readable_theme

if os.environ.get('READTHEDOCS', None) == 'True':
    os.environ['PATH'] += os.pathsep + os.path.abspath('_bin')
    os.environ['LD_LIBRARY_PATH'] = os.path.abspath('_bin')

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    #'sphinx.ext.doctest',
    'sphinx.ext.intersphinx',
    'sphinx.ext.mathjax',
    #'sphinx.ext.pngmath',
    'sphinx.ext.viewcode',
    'sphinxcontrib.tikz',
    'numpydoc',
    ]
autosummary_generate = True
autodoc_default_flags = ['members']
numpydoc_show_class_members = True
numpydoc_show_inherited_class_members = True
source_suffix = '.rst'
source_encoding = 'utf-8-sig'
master_doc = 'index'
project = u'kohonen'
copyright = u'2014, Leif Johnson'
version = '1.1'
release = '1.1.2'
exclude_patterns = ['_build']
pygments_style = 'tango'

html_theme = 'readable'
html_theme_path = [sphinx_readable_theme.get_html_theme_path()]
htmlhelp_basename = 'kohonendoc'

latex_elements = {
#'papersize': 'letterpaper',
#'pointsize': '10pt',
'preamble': r'''
\usepackage{tikz}
\usepackage{pgfplots}
\usetikzlibrary{arrows}''',
}

intersphinx_mapping = {
    'python': ('http://docs.python.org/', None),
    'numpy': ('http://docs.scipy.org/doc/numpy/', None),
    'scipy': ('http://docs.scipy.org/doc/scipy/reference/', None),
}

