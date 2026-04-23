# Copyright (C) 2026 duyongquan <quandy2020@126.com>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# This module provides function for joining paths
# known from most languages

function(join_paths joined_path first_path_segment)
  set(temp_path "${first_path_segment}")
  foreach(current_segment IN LISTS ARGN)
    if(NOT ("${current_segment}" STREQUAL ""))
      if(IS_ABSOLUTE "${current_segment}")
        set(temp_path "${current_segment}")
      else()
        set(temp_path "${temp_path}/${current_segment}")
      endif()
    endif()
  endforeach()
  set(${joined_path} "${temp_path}" PARENT_SCOPE)
endfunction()
