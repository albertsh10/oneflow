from __future__ import absolute_import

import oneflow.python.framework.python_callback as python_callback
import oneflow.python.eager.interpreter_callback as interpreter_callback
import oneflow.python.framework.c_api_util as c_api_util

python_callback.interpreter_callback = interpreter_callback
c_api_util.RegisterForeignCallbackOnlyOnce(python_callback.global_python_callback)