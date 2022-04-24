#pragma once

#define DISABLE_LVALUE_COPY_AND_ASSIGN(Class) Class(const Class &) = delete; Class& operator=(const Class&) = delete;
#define DISABLE_RVALUE_COPY_AND_ASSIGN(Class) Class(const Class &&) = delete; Class& operator=(const Class&&) = delete;
#define DISABLE_COPY_AND_ASSIGN(Class) DISABLE_LVALUE_COPY_AND_ASSIGN(Class) DISABLE_RVALUE_COPY_AND_ASSIGN(Class)