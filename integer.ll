; ModuleID = 'Integer'
source_filename = "integer"


declare void @Boolean_Constructor_Boolean(ptr %0, i1 %1)

; Declare the Boolean type here as it is defined in the other file
%Boolean = type { i1 }

%Integer = type { i32 }

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1  ; Format for printing integers
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1 ; Format for scanning integers

; Declaration for the printf and scanf functions
declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

; Function to create a default Integer object with a given value
define void @Integer_Constructor_Integer(ptr %0, i32 %1) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}

define i32 @Integer_get(ptr %integerPtr) {
entry:
  ; Get a pointer to the field of the %Integer object
  %fieldPtr = getelementptr inbounds %Integer, ptr %integerPtr, i32 0, i32 0

  ; Load the integer value from the %Integer object
  %value = load i32, ptr %fieldPtr, align 4

  ; Return the integer value
  ret i32 %value
}

define %Integer @Integer_Mult_Integer(ptr %self, %Integer %other) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Get the value of the other Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Multiply the values
  %result = mul i32 %selfValue, %otherValue

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
}

define %Integer @Integer_Plus_Integer(ptr %self, %Integer %other) {
entry:
  ; Get a pointer to the field of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0

  ; Load the current value of the self Integer object
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Create a temporary memory location to store %other since it's a value, not a pointer
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4

  ; Get a pointer to the field of the other %Integer object
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0

  ; Load the integer value from the other %Integer object
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Add the values
  %result = add i32 %selfValue, %otherValue

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
}

define %Integer @Integer_Minus_Integer(ptr %self, %Integer %other) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Get the value of the other Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Subtract the values
  %result = sub i32 %selfValue, %otherValue

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
}

define %Integer @Integer_Div_Integer(ptr %self, %Integer %other) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Get the value of the other Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Perform division
  %result = sdiv i32 %selfValue, %otherValue

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
}


define %Integer @Integer_Rem_Integer(ptr %self, %Integer %other) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Get the value of the other Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Perform remainder operation
  %result = srem i32 %selfValue, %otherValue

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
}

define %Boolean @Integer_Less_Integer(ptr %self, %Integer %other) {
entry:
  ; Get a pointer to the field of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0

  ; Load the value of the self Integer object
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Create a temporary memory location to store %other
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4

  ; Get a pointer to the field of the other Integer object
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0

  ; Load the value of the other Integer object
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Perform the less-than comparison
  %result = icmp slt i32 %selfValue, %otherValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ; Return the Boolean result
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater_Integer(ptr %self, %Integer %other) {
entry:
  ; Get a pointer to the field of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0

  ; Load the value of the self Integer object
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Create a temporary memory location to store %other
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 4

  ; Get a pointer to the field of the other Integer object
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0

  ; Load the value of the other Integer object
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Perform the greater-than comparison
  %result = icmp sgt i32 %selfValue, %otherValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load the value of %booleanResult and return it
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1

  ; Return the loaded %Boolean value (not the pointer)
  ret %Boolean %loadedBoolean
}


; Function to print the Integer object
define void @Integer_print(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %loadInt = load i32, ptr %intFieldPtr, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, i32 %loadInt)
  ret void
}

; Function to scan an integer value from the console and store it in the Integer object

@error_msg = private unnamed_addr constant [33 x i8] c"Invalid input. Exiting program.\0A\00", align 1 ; Custom error message
define void @Integer_scan(ptr %0) {
entry:
  ; Define a pointer to the Integer field and the format string for scanf
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0

  ; Call scanf to read the integer value from the console
  ; scanf expects a pointer to where the value should be stored
  %scanResult = call i32 (ptr, ...) @scanf(ptr @fmt_scan, ptr %intFieldPtr)

  ; Check if scanf successfully scanned one item (integer)
  ; If scanResult is not 1, it means an invalid input was given
  %ifcond = icmp eq i32 %scanResult, 1
  br i1 %ifcond, label %validInput, label %invalidInput

validInput:
  ret void

invalidInput:
  call i32 (ptr, ...) @printf(ptr @error_msg)
  call void @exit(i32 1)
  ret void

}

; Declare the exit function to terminate the program
declare void @exit(i32)
