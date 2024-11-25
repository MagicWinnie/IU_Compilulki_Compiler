; ModuleID = 'Boolean'
source_filename = "boolean"

%Boolean = type { i1 }

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1


define void @Boolean_Init(ptr %0) {
entry:
  ret void
}

define void @Boolean_Constructor_Boolean(ptr %0, i1 %1) {
entry:
  call void @Boolean_Init(ptr %0)
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 %1, ptr %boolFieldPtr, align 1
  ret void
}


define void @Boolean_Constructor(ptr %0) {
entry:
call void @Boolean_Init(ptr %0)
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 0, ptr %boolFieldPtr, align 1
  ret void
}


; Define the 'Or' function
define %Boolean @Boolean_Or_Boolean(ptr %self, %Boolean %other) {
entry:
  ; Load the boolean value from 'self' and 'other'
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1

  %otherFieldValue = extractvalue %Boolean %other, 0

  ; Perform logical OR operation
  %orResult = or i1 %selfValue, %otherFieldValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %orResult)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean

}

define %Boolean @Boolean_And_Boolean(ptr %self, i1 %other) {
entry:
  ; Load the boolean value from 'self' and 'other'
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1

  ; Perform logical AND operation
  %andResult = and i1 %selfValue, %other

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %andResult)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

; Define the 'Not' function (negation)
define %Boolean @Boolean_Not(ptr %self) {
entry:
  ; Load the boolean value from 'self'
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1

  ; Perform logical NOT operation (negation)
  %notResult = xor i1 %selfValue, true

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %notResult)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

; Define the 'Xor' function (exclusive OR)
define %Boolean @Boolean_Xor_Boolean(ptr %self, i1 %other) {
entry:
  ; Load the boolean value from 'self' and 'other'
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1

  ; Perform logical XOR operation
  %xorResult = xor i1 %selfValue, %other

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %xorResult)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}



define void @Boolean_print(ptr %0) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  %loadBool = load i1, ptr %boolFieldPtr, align 1

  ; Conditional branching based on the value of the boolean (i1)
  br i1 %loadBool, label %print_true, label %print_false

print_true:
  ; If the value is true (1), print "true"
  %true_ptr = getelementptr inbounds [5 x i8], ptr @true_str, i32 0, i32 0
  call i32 (ptr, ...) @printf(ptr @fmt, ptr %true_ptr)
  ret void

print_false:
  ; If the value is false (0), print "false"
  %false_ptr = getelementptr inbounds [6 x i8], ptr @false_str, i32 0, i32 0
  call i32 (ptr, ...) @printf(ptr @fmt, ptr %false_ptr)
  ret void
}


declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

