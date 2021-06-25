# -*- coding: utf-8 -*-
#
# SMTK documentation build configuration file, created by
# sphinx-quickstart on Thu Sep 18 18:53:31 2014.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.


import sys
import os
import datetime

# build results: https://readthedocs.org/projects/smtk/builds/
# test locally, something like:
#   cd doc
#   python3 -m sphinx -Q -b html . html
# you can also `export READTHEDOCS=1` beforehand so it pretends it's on RTD.
readTheDocs = os.environ.get('READTHEDOCS', None) != None
localReadTheDocs = os.environ.get('LOCALREADTHEDOCS', None) != None
localSkipDoxygen = os.environ.get('LOCALSKIPDOXYGEN', None) != None
sys.path.append(os.getcwd())  # So that the findfigure package can be imported
# FIXME: Is the penultimate argument always the source dir?
sourcedir = sys.argv[-2]
# FIXME: Is the final argument always be the build dir?
builddir = sys.argv[-1]


def setup(app):
    # prevent stupid-wide table columns.
    app.add_stylesheet("theme-overrides.css")


def runDoxygen(rtdsrcdir, rtdblddir, doxyfileIn, doxyfileOut):
    """Run Doxygen as part of generating user documentation.

    This is only meant to be used on readthedocs.org to generate
    reference documentation for linking into the user's guide and
    tutorial. It should eventually be replaced by something that
    fetches tag files, XML files, and references remotely-generated
    documentation from an actual SMTK build.
    """
    import re
    import subprocess
    dxiname = open(os.path.join(rtdsrcdir, doxyfileIn), 'r')
    cfg = dxiname.read()
    orgdir = os.path.abspath(os.getcwd())
    srcdir = os.path.abspath(os.path.join(os.getcwd(), '..'))
    bindir = srcdir
    refdir = os.path.abspath(os.path.join(rtdblddir, 'doc', 'reference'))
    # We want to direct the output differently, and since there's no smtk
    # build, have the bin dir just point to the source - so replace output
    # first, then bin dir.
    cfg2 = re.sub('@smtk_SOURCE_DIR@', srcdir,
                  re.sub('@smtk_BINARY_DIR@', os.path.abspath(bindir),
                         re.sub('@smtk_BINARY_DIR@/doc/reference/smtk', os.path.abspath(rtdblddir) + '/doc/reference/smtk',
                                cfg)))
    try:
        os.makedirs(refdir)
    except OSError as e:
        if e.errno == 17:
            pass
    except:
        print('Failed to create doxygen reference directory %s' % refdir)
        return
    dxoname = os.path.abspath(os.path.join(refdir, doxyfileOut))
    dxo = open(dxoname, 'w')
    print(cfg2, file=dxo)
    dxo.close()

    # copied from https://test-builds.readthedocs.io/en/git-lfs/conf.html
    # Install and execute git-lfs
    if not os.path.exists('./git-lfs'):
        print('Fetching files with git-lfs')
        os.system(
            'wget https://github.com/git-lfs/git-lfs/releases/download/v2.7.1/git-lfs-linux-amd64-v2.7.1.tar.gz')
        os.system('tar xvfz git-lfs-linux-amd64-v2.7.1.tar.gz')
        # make lfs available, but don't install hooks!
        # they will prevent checkout working a second time without git-lfs installed.
        os.system('./git-lfs install --skip-repo')
        os.system('./git-lfs fetch')  # download content from remote
        # make local files to have the real content on them
        os.system('./git-lfs checkout')
        # make sure the problematic hook is gone: .git/hooks/post-checkout
        try:
            os.remove(os.path.abspath(os.path.join(
                srcdir, '.git', 'hooks', 'post-checkout')))
        except OSError:
            pass

    os.chdir(refdir)
    print('Running Doxygen on %s' % dxoname)
    rcode = subprocess.call(('doxygen', dxoname))
    print('   Doxygen returned %s' % rcode)
    os.chdir(orgdir)


def configFile(srcdir, blddir, templateFile, outputFile, keywords):
    ofile = open(os.path.join(srcdir, outputFile), 'w')
    data = open(os.path.join(srcdir, templateFile), 'r').read()
    for patt in keywords:
        repl = keywords[patt]
        data = data.replace(patt, str(repl))
    ofile.write(data)
    ofile.close()


if readTheDocs or localReadTheDocs:
    """Configure files and run Doxygen ourselves"""
    # Configure some files
    configFile(sourcedir, builddir, '../CMake/Version.h.in', '../smtk/common/Version.h', {
        '@SMTK_VERSION_MAJOR@': 1,
        '@SMTK_VERSION_MINOR@': 1,
        '@SMTK_VERSION_PATCH@': 0,
        '@SMTK_VERSION@': '1.1.0'})
    # Run doxygen ourselves on ReadTheDocs.org so that doxylinks will work.
    if not localSkipDoxygen:
        runDoxygen(sourcedir, builddir, 'smtk.doxyfile.in', 'smtk.doxyfile')

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
# sys.path.insert(0, os.path.abspath('.'))

# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.todo',
    'sphinxcontrib.actdiag',
    'sphinxcontrib.doxylink',
    'findfigure',
    'breathe'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['.templates']

# The suffix of source filenames.
source_suffix = '.rst'

# The encoding of source files.
# source_encoding = 'utf-8-sig'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = 'SMTK'
year = datetime.datetime.now().year
copyright = '%d, Kitware, Inc.' % year


def readVersionInfo(srcdir):
    import re
    pattern = re.compile('([0-9]+)\\.([0-9]+)(\\.([0-9]+)(-(.*))?)?')
    vinfo = open(os.path.join(srcdir, '..', 'version.txt'), 'r').read().strip()
    m = pattern.match(vinfo).groups()
    major = int(m[0])
    minor = int(m[1])
    patch = int(m[3] or '0')
    label = m[5]
    # The version info for the project you're documenting, acts as replacement for
    # |version| and |release|, also used in various other places throughout the
    # built documents.
    #
    # The short X.Y version.
    version = '%s.%s' % (major, minor)
    # The full version, including alpha/beta/rc tags.
    release = vinfo
    return (version, release)


version, release = readVersionInfo(sourcedir)

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
# language = None

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
# today = ''
# Else, today_fmt is used as the format for a strftime call.
# today_fmt = '%B %d, %Y'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.

# We exclude files (from being reported as unreferenced) that are explicitly included.
# We explicitly include files that we want to appear as introductory text above a
# table of contents listing.
exclude_patterns = ['userguide-overview.rst']


# The reST default role (used for this markup: `text`) to use for all
# documents.
# default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
# add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
# add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
# show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# A list of ignored prefixes for module index sorting.
# modindex_common_prefix = []

# If true, keep warnings as "system message" paragraphs in the built documents.
# keep_warnings = False

# -- Findfigure configuration ---------------------------------------------

findfigure_paths = {
    '*': [
        sourcedir,
        os.path.join(sourcedir, 'images'),
        os.path.join(sourcedir, 'userguide', 'figures'),
        builddir,
        os.path.join(builddir, '..')]
}

# -- Action diagram configuration -----------------------------------------

actdiag_antialias = True

actdiag_html_image_format = 'SVG'
# using PDF format causes a warning about fonts that produces nothing when
# using sphinx 2, setting actdiag_fontpath to a TTF might fix it?
actdiag_latex_image_format = 'PNG'
# actdiag_debug = True

# -- To-do list configuration ---------------------------------------------

# When True, ..todo:: and ..todolist:: produce output, else they produce
# nothing. The default is False.
todo_include_todos = True

# -- Doxylink configuration -----------------------------------------------

# The doxylink environment is set up with a dictionary mapping
# the interpereted text role to a tuple of tag file and prefix:
tagbase = os.path.join(builddir, '..', '..', 'reference')
refbase = os.path.join('..', '..', 'reference')
if readTheDocs:
    # We store the reference documentation inside the user-doc build
    # directory on readthedocs so that it will get installed properly.
    tagbase = os.path.abspath(os.path.join(builddir, 'doc', 'reference'))
    refbase = os.path.join('doc', 'reference')
doxylink = {
    'smtk': (
        os.path.join(tagbase, 'smtk.tags'),
        os.path.join(refbase, 'smtk', 'html'))
}

# A boolean that decides whether parentheses are appended to
# function and method role text. Default is True.
# add_function_parentheses = True


# -- Breath configuration -------------------------------------------------

breathe_projects = {
    'smtk': os.path.join(builddir, '..', '..', 'reference', 'smtk', 'xml') + os.path.sep,
}
breathe_default_project = 'smtk'
breathe_default_members = ('members', 'protected-members', 'undoc-members')


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
if readTheDocs:
    html_theme = 'default'
else:
    try:
        import sphinx_rtd_theme
        html_theme = 'sphinx_rtd_theme'
        html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
    except:
        html_theme = 'haiku'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
# html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
# html_theme_path = []

# The name for this set of Sphinx documents.  If None, it defaults to
# "<project> v<release> documentation".
# html_title = None
html_title = 'SMTK: Simulation Modeling Tool Kit, v1.0'

# A shorter title for the navigation bar.  Default is the same as html_title.
# html_short_title = None

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
# html_logo = None

# The name of an image file (within the static path) to use as favicon of the
# docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
# html_favicon = None

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['.static']

# Add any extra paths that contain custom files (such as robots.txt or
# .htaccess) here, relative to this directory. These files are copied
# directly to the root of the documentation.
# html_extra_path = []

# If not '', a 'Last updated on:' timestamp is inserted at every page bottom,
# using the given strftime format.
# html_last_updated_fmt = '%b %d, %Y'

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
# html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
# html_sidebars = {}

# Additional templates that should be rendered to pages, maps page names to
# template names.
# html_additional_pages = {}

# If false, no module index is generated.
# html_domain_indices = True

# If false, no index is generated.
# html_use_index = True

# If true, the index is split into individual pages for each letter.
# html_split_index = False

# If true, links to the reST sources are added to the pages.
# html_show_sourcelink = True

# If true, "Created using Sphinx" is shown in the HTML footer. Default is True.
# html_show_sphinx = True

# If true, "(C) Copyright ..." is shown in the HTML footer. Default is True.
# html_show_copyright = True

# If true, an OpenSearch description file will be output, and all pages will
# contain a <link> tag referring to it.  The value of this option must be the
# base URL from which the finished HTML is served.
# html_use_opensearch = ''

# This is the file name suffix for HTML files (e.g. ".xhtml").
# html_file_suffix = None

# Output file base name for HTML help builder.
htmlhelp_basename = 'SMTKdoc'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    # 'preamble': '',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    ('index', 'SMTK.tex', 'SMTK Documentation',
     'Kitware, Inc.', 'manual'),
]

# The name of an image file (relative to this directory) to place at the top of
# the title page.
# latex_logo = None

# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
# latex_use_parts = False

# If true, show page references after internal links.
# latex_show_pagerefs = False

# If true, show URL addresses after external links.
# latex_show_urls = False

# Documents to append as an appendix to all manuals.
# latex_appendices = []

# If false, no module index is generated.
# latex_domain_indices = True


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    ('index', 'smtk', 'SMTK Documentation',
     ['Kitware, Inc.'], 1)
]

# If true, show URL addresses after external links.
# man_show_urls = False


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    ('index', 'SMTK', 'SMTK Documentation',
     'Kitware, Inc.', 'SMTK', 'One line description of project.',
     'Miscellaneous'),
]

# Documents to append as an appendix to all manuals.
# texinfo_appendices = []

# If false, no module index is generated.
# texinfo_domain_indices = True

# How to display URL addresses: 'footnote', 'no', or 'inline'.
# texinfo_show_urls = 'footnote'

# If true, do not generate a @detailmenu in the "Top" node's menu.
# texinfo_no_detailmenu = False

# -- Options for Epub output ----------------------------------------------

# Bibliographic Dublin Core info.
epub_title = 'SMTK: Simulation Modeling Tool Kit'
epub_author = 'Kitware, Inc.'
epub_publisher = 'Kitware, Inc.'
epub_copyright = copyright

# The basename for the epub file. It defaults to the project name.
# epub_basename = u'SMTK'

# The HTML theme for the epub output. Since the default themes are not optimized
# for small screen space, using the same theme for HTML and epub output is
# usually not wise. This defaults to 'epub', a theme designed to save visual
# space.
# epub_theme = 'epub'

# The language of the text. It defaults to the language option
# or en if the language is not set.
# epub_language = ''

# The scheme of the identifier. Typical schemes are ISBN or URL.
# epub_scheme = ''

# The unique identifier of the text. This can be a ISBN number
# or the project homepage.
# epub_identifier = ''

# A unique identification for the text.
# epub_uid = ''

# A tuple containing the cover image and cover page html template filenames.
# epub_cover = ()

# A sequence of (type, uri, title) tuples for the guide element of content.opf.
# epub_guide = ()

# HTML files that should be inserted before the pages created by sphinx.
# The format is a list of tuples containing the path and title.
# epub_pre_files = []

# HTML files shat should be inserted after the pages created by sphinx.
# The format is a list of tuples containing the path and title.
# epub_post_files = []

# A list of files that should not be packed into the epub file.
epub_exclude_files = ['search.html']

# The depth of the table of contents in toc.ncx.
# epub_tocdepth = 3

# Allow duplicate toc entries.
# epub_tocdup = True

# Choose between 'default' and 'includehidden'.
# epub_tocscope = 'default'

# Fix unsupported image types using the PIL.
# epub_fix_images = False

# Scale large images.
# epub_max_image_width = 0

# How to display URL addresses: 'footnote', 'no', or 'inline'.
# epub_show_urls = 'inline'

# If false, no index is generated.
# epub_use_index = True
