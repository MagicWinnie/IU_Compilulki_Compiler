; ModuleID = 'Real'
source_filename = "real"

declare void @Boolean_Constructor_Boolean(ptr %0, i1 %1)
declare void @Integer_Constructor_Integer(ptr %0, i32 %1)

define void @Real_Init(ptr %0) {
entry:
  ret void
}

%Boolean = type { i1 }
%Real = type { double }
%Integer = type { i32 }

@fmt = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@fmt_scan_real = private unnamed_addr constant [3 x i8] c"%f\00", align 1 ; Format for scanning doubles
@error_msg = private unnamed_addr constant [33 x i8] c"Invalid input. Exiting program.\0A\00", align 1 ; Custom error message


define void @Real_Constructor_Real(ptr %0, double %1) {
entry:
  call void @Real_Init(ptr %0)
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  store double %1, ptr %doubleFieldPtr, align 8
  ret void
}

define void @Real_Constructor(ptr %0) {
entry:
  call void @Real_Init(ptr %0)
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
    store double 0.0, ptr %doubleFieldPtr, align 8
  ret void
}

define void @Real_Constructor_Integer(ptr %realPtr, %Integer %integerValue) {
entry:
  call void @Real_Init(ptr %realPtr)
  ; Extract the i32 value from the %Integer type
  %integerFieldValue = extractvalue %Integer %integerValue, 0

  ; Convert the extracted i32 value to double
  %realValue = sitofp i32 %integerFieldValue to double

  ; Store the converted value into the Real object
  %realFieldPtr = getelementptr inbounds %Real, ptr %realPtr, i32 0, i32 0
  store double %realValue, ptr %realFieldPtr, align 8

  ret void
}

define %Integer @Real_toInteger(ptr %self) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Convert the double to an integer
  %convertedValue = fptosi double %selfValue to i32

  ; Allocate memory for the Integer result
  %resultPtr = alloca %Integer, align 4

  ; Create the Integer object
  call void @Integer_Constructor_Integer(ptr %resultPtr, i32 %convertedValue)

  ; Load and return the Integer result
  %loadedResult = load %Integer, ptr %resultPtr, align 4
  ret %Integer %loadedResult
}

define %Real @Real_UnaryMinus(ptr %self) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform negation
  %negatedValue = fsub double %selfValue, 1.0

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %negatedValue)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}





define %Real @Real_Plus_Real(ptr %self, %Real %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the double value from the `other` Real object
  %otherPtr = alloca %Real
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8

  ; Perform the addition
  %result = fadd double %selfValue, %otherValue

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Minus_Real(ptr %self, %Real %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the double value from the `other` Real object
  %otherPtr = alloca %Real
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8

  ; Perform the addition
  %result = fsub double %selfValue, %otherValue

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Mult_Real(ptr %self, %Real %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the double value from the `other` Real object
  %otherPtr = alloca %Real
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8

  ; Perform the addition
  %result = fmul double %selfValue, %otherValue

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Div_Real(ptr %self, %Real %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the double value from the `other` Real object
  %otherPtr = alloca %Real
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8

  ; Perform the addition
  %result = fdiv double %selfValue, %otherValue

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Plus_Integer(ptr %self, %Integer %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the integer value from the `other` Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %otherValue to double

  ; Perform the addition
  %result = fadd double %selfValue, %otherAsReal

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Minus_Integer(ptr %self, %Integer %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the integer value from the `other` Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %otherValue to double

  ; Perform the addition
  %result = fsub double %selfValue, %otherAsReal

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Mult_Integer(ptr %self, %Integer %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the integer value from the `other` Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %otherValue to double

  ; Perform the addition
  %result = fmul double %selfValue, %otherAsReal

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Div_Integer(ptr %self, %Integer %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the integer value from the `other` Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %otherValue to double

  ; Perform the addition
  %result = fdiv double %selfValue, %otherAsReal

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Rem_Integer(ptr %self, %Integer %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Extract the integer value from the `other` Integer object
  %otherPtr = alloca %Integer
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4

  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %otherValue to double

  ; Perform the addition
  %result = frem double %selfValue, %otherAsReal

  ; Allocate memory for the result Real
  %resultPtr = alloca %Real, align 8

  ; Create the result Real object
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)

  ; Load and return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}



; Function: Real_Min
; Returns the most negative finite value for a `double`
define %Real @Real_Min() {
entry:
  ; Get the most negative finite value for `double` (approx -1.7976931348623157e+308)
  %minValue = fsub double 0.0, 1.7976931348623157e+308

  ; Allocate memory for the result Real object
  %resultPtr = alloca %Real, align 8

  ; Construct the Real object with the min value
  call void @Real_Constructor_Real(ptr %resultPtr, double %minValue)

  ; Return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

; Function: Real_Max
; Returns the largest positive finite value for a `double`
define %Real @Real_Max() {
entry:
  ; Get the maximum positive finite value for `double` (approx 1.7976931348623157e+308)
  %maxValue = fadd double 0.0, 1.7976931348623157e+308

  ; Allocate memory for the result Real object
  %resultPtr = alloca %Real, align 8

  ; Construct the Real object with the max value
  call void @Real_Constructor_Real(ptr %resultPtr, double %maxValue)

  ; Return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

; Function: Real_Epsilon
; Returns the smallest value `e` such that `1.0 + e > 1.0` for a `double`
define %Real @Real_Epsilon() {
entry:
  ; Get the epsilon value for `double` (approx 2.220446049250313e-16)
  %epsilonValue = fadd double 0.0, 2.220446049250313e-16

  ; Allocate memory for the result Real object
  %resultPtr = alloca %Real, align 8

  ; Construct the Real object with the epsilon value
  call void @Real_Constructor_Real(ptr %resultPtr, double %epsilonValue)

  ; Return the Real result
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}




define %Boolean @Real_Less_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform the less-than comparison
  %result = fcmp olt double %selfValue, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Greater_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform the less-than comparison
  %result = fcmp ogt double %selfValue, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_LessEqual_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform the less-than comparison
  %result = fcmp ole double %selfValue, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_GreaterEqual_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform the less-than comparison
  %result = fcmp oge double %selfValue, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Equal_Real(ptr %self, double %realValue) {
entry:
  ; Get the value of the self Integer object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8

  ; Perform the less-than comparison
  %result = fcmp oeq double %selfValue, %realValue

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Less_Integer(ptr %self, i32 %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8


  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %other to double

  ; Perform the less-than comparison
  %result = fcmp olt double %selfValue, %otherAsReal

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_LessEqual_Integer(ptr %self, i32 %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8


  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %other to double

  ; Perform the less-than comparison
  %result = fcmp ole double %selfValue, %otherAsReal

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Greater_Integer(ptr %self, i32 %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8


  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %other to double

  ; Perform the less-than comparison
  %result = fcmp ogt double %selfValue, %otherAsReal

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_GreaterEqual_Integer(ptr %self, i32 %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8


  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %other to double

  ; Perform the less-than comparison
  %result = fcmp oge double %selfValue, %otherAsReal

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Equal_Integer(ptr %self, i32 %other) {
entry:
  ; Extract the double value from the `self` Real object
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8


  ; Convert the Integer value to double
  %otherAsReal = sitofp i32 %other to double

  ; Perform the less-than comparison
  %result = fcmp oeq double %selfValue, %otherAsReal

  ; Create a Boolean using Boolean_Constructor_Boolean, passing the comparison result
  %booleanResult = alloca %Boolean
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)

  ; Load and return the Boolean result
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}



define void @Real_print(ptr %0) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  %loadDouble = load double, ptr %doubleFieldPtr, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, double %loadDouble)
  ret void
}

declare i32 @printf(ptr, ...)
declare i32 @scanf(ptr, ...)
