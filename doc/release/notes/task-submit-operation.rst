Task System
-----------

A Task for Submitting Operations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is a new :smtk:`smtk::task::SubmitOperation` class for
situations where users must prepare an operation to be run
(or should run an operation iteratively until satisfied).
See `task-submit-operation`_ for more information.

The operation parameter-editor panel responds to this new
task by displaying the operation parameters corresponding
to the task.
See `smtk-pv-parameter-editor-panel`_ for more information.


An adaptor for configuring SubmitOperation tasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is a new :smtk:``smtk::task::adaptor::ConfigureOperation``
class for situations where parts of an SMTK operation
managed by a :smtk:``smtk::task::SubmitOperation`` are
configured by an upstream :smtk:``smtk::task::FillOutAttributes``
task.
