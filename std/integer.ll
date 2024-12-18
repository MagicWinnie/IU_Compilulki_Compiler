; ModuleID = 'Integer'
source_filename = "integer"


declare void @Boolean_Constructor_Boolean(ptr %0, i1 %1)
declare void @Real_Constructor_Real(ptr %0, double %1)

; Declare the Boolean type here as it is defined in the other file
%Boolean = type { i1 }

%Real = type { double }

%Integer = type { i32 }


@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1  ; Format for printing integers
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1 ; Format for scanning integers

; Declaration for the printf and scanf functions
declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)

define void @Integer_Init(ptr %0) {
entry:
  ret void
}

; Function to create a default Integer object with a given value
define void @Integer_Constructor_Integer(ptr %0, i32 %1) {
entry:
  call void @Integer_Init(ptr %0)
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}


define void @Integer_Constructor(ptr %0) {
entry:
  call void @Integer_Init(ptr %0)
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 0, ptr %intFieldPtr, align 4
  ret void
}


; Constructor: Integer from Real
define void @Integer_Constructor_Real(ptr %0, double %1) {
entry:
   call void @Integer_Init(ptr %0)
  ; Convert the real value to an integer
  %intValue = fptosi double %1 to i32

  ; Get the pointer to the integer field of the object
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0

  ; Store the converted integer value
  store i32 %intValue, ptr %intFieldPtr, align 4
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

; Function: Integer Min
define %Integer @Integer_Min(ptr %self) {
entry:
  ; Return an Integer object with the minimum value (-2147483648)
  %minValue = insertvalue %Integer undef, i32 -2147483648, 0
  ret %Integer %minValue
}

; Function: Integer Max
define %Integer @Integer_Max(ptr %self) {
entry:
  ; Return an Integer object with the maximum value (2147483647)
  %maxValue = insertvalue %Integer undef, i32 2147483647, 0
  ret %Integer %maxValue
}

; Function: Integer toReal
; Converts an Integer to a Real (double)
define %Real @Integer_toReal(ptr %self) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the integer value to a double
  %selfAsReal = sitofp i32 %selfValue to double

  ; Allocate space for the Real result
  %realResult = alloca %Real, align 8

  ; Call the Real constructor to create the Real object with the converted value
  call void @Real_Constructor_Real(ptr %realResult, double %selfAsReal)

  ; Load the Real object and return it
  %loadedReal = load %Real, ptr %realResult, align 8
  ret %Real %loadedReal
}

; Function: Integer toBoolean
; Converts an Integer to a Boolean
define %Boolean @Integer_toBoolean(ptr %self) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Check if the integer value is not zero
  %isNonZero = icmp ne i32 %selfValue, 0

  ; Allocate space for the Boolean result
  %booleanResult = alloca %Boolean

  ; Call the Boolean constructor to create the Boolean object
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %isNonZero)

  ; Load the constructed Boolean object
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1

  ; Return the Boolean object
  ret %Boolean %loadedBoolean
}

define %Integer @Integer_UnaryMinus(ptr %self) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Subtract the values
  %result = sub i32 %selfValue, 1

  ; Create a new Integer value
  %returnValue = insertvalue %Integer undef, i32 %result, 0

  ; Return the new Integer object as a value
  ret %Integer %returnValue
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

; Function: Integer Mult Real
define %Real @Integer_Mult_Real(ptr %self, %Real %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert selfValue to double
  %selfAsReal = sitofp i32 %selfValue to double

  ; Extract the double value from the %Real object (realValue)
  %realValueAsDouble = extractvalue %Real %realValue, 0

  ; Perform addition
  %result = fmul double %selfAsReal, %realValueAsDouble

  ; Allocate space for the Real object (pointer to Real)
  %realResult = alloca %Real, align 8

  ; Call the Real constructor to create the Real object with the result
  call void @Real_Constructor_Real(ptr %realResult, double %result)

  ; Load the Real object and return it
  %loadedReal = load %Real, ptr %realResult, align 8
  ret %Real %loadedReal
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

; Function: Integer Plus Real
define %Real @Integer_Plus_Real(ptr %self, %Real %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert selfValue to double
  %selfAsReal = sitofp i32 %selfValue to double

  ; Extract the double value from the %Real object (realValue)
  %realValueAsDouble = extractvalue %Real %realValue, 0

  ; Perform addition
  %result = fadd double %selfAsReal, %realValueAsDouble

  ; Allocate space for the Real object (pointer to Real)
  %realResult = alloca %Real, align 8

  ; Call the Real constructor to create the Real object with the result
  call void @Real_Constructor_Real(ptr %realResult, double %result)

  ; Load the Real object and return it
  %loadedReal = load %Real, ptr %realResult, align 8
  ret %Real %loadedReal
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

; Function: Integer Minus Real
define %Real @Integer_Minus_Real(ptr %self, %Real %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert selfValue to double
  %selfAsReal = sitofp i32 %selfValue to double

  ; Extract the double value from the %Real object (realValue)
  %realValueAsDouble = extractvalue %Real %realValue, 0

  ; Perform addition
  %result = fsub double %selfAsReal, %realValueAsDouble

  ; Allocate space for the Real object (pointer to Real)
  %realResult = alloca %Real, align 8

  ; Call the Real constructor to create the Real object with the result
  call void @Real_Constructor_Real(ptr %realResult, double %result)

  ; Load the Real object and return it
  %loadedReal = load %Real, ptr %realResult, align 8
  ret %Real %loadedReal
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

; Function: Integer Div Real
define %Real @Integer_Div_Real(ptr %self, %Real %realValue) {
entry:
; Get the value of the self Integer object
%selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
%selfValue = load i32, ptr %selfFieldPtr, align 4

; Convert selfValue to double
%selfAsReal = sitofp i32 %selfValue to double

; Extract the double value from the %Real object (realValue)
%realValueAsDouble = extractvalue %Real %realValue, 0

; Perform addition
%result = fdiv double %selfAsReal, %realValueAsDouble

; Allocate space for the Real object (pointer to Real)
%realResult = alloca %Real, align 8

; Call the Real constructor to create the Real object with the result
call void @Real_Constructor_Real(ptr %realResult, double %result)

; Load the Real object and return it
%loadedReal = load %Real, ptr %realResult, align 8
ret %Real %loadedReal
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

define %Boolean @Integer_LessEqual_Integer(ptr %self, %Integer %other) {
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
  %result = icmp sle i32 %selfValue, %otherValue

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

define %Boolean @Integer_GreaterEqual_Integer(ptr %self, %Integer %other) {
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
  %result = icmp sge i32 %selfValue, %otherValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load the value of %booleanResult and return it
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1

  ; Return the loaded %Boolean value (not the pointer)
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Equal_Integer(ptr %self, %Integer %other) {
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
  %result = icmp eq i32 %selfValue, %otherValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load the value of %booleanResult and return it
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1

  ; Return the loaded %Boolean value (not the pointer)
  ret %Boolean %loadedBoolean
}




define %Boolean @Integer_Less_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the Integer value to a double (Real)
  %selfAsReal = sitofp i32 %selfValue to double

  ; Perform the less-than comparison
  %result = fcmp olt double %selfAsReal, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_LessEqual_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the Integer value to a double (Real)
  %selfAsReal = sitofp i32 %selfValue to double

  ; Perform the less-than comparison
  %result = fcmp ole double %selfAsReal, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the Integer value to a double (Real)
  %selfAsReal = sitofp i32 %selfValue to double

  ; Perform the less-than comparison
  %result = fcmp ogt double %selfAsReal, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_GreaterEqual_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the Integer value to a double (Real)
  %selfAsReal = sitofp i32 %selfValue to double

  ; Perform the less-than comparison
  %result = fcmp oge double %selfAsReal, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Equal_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4

  ; Convert the Integer value to a double (Real)
  %selfAsReal = sitofp i32 %selfValue to double

  ; Perform the less-than comparison
  %result = fcmp oeq double %selfAsReal, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
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
