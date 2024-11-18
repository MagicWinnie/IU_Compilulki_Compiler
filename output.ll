; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }
%IntArray = type { ptr, i32 }
%Main = type {}

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@error_msg = private unnamed_addr constant [33 x i8] c"Invalid input. Exiting program.\0A\00", align 1
@fmt.4 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@int_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

define void @Boolean_Constructor_Boolean(ptr %0, i1 %1) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 %1, ptr %boolFieldPtr, align 1
  ret void
}

define void @Boolean_Constructor(ptr %0) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 false, ptr %boolFieldPtr, align 1
  ret void
}

define %Boolean @Boolean_Or_Boolean(ptr %self, %Boolean %other) {
entry:
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1
  %otherFieldValue = extractvalue %Boolean %other, 0
  %orResult = or i1 %selfValue, %otherFieldValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %orResult)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Boolean_And_Boolean(ptr %self, i1 %other) {
entry:
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1
  %andResult = and i1 %selfValue, %other
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %andResult)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Boolean_Not(ptr %self) {
entry:
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1
  %notResult = xor i1 %selfValue, true
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %notResult)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Boolean_Xor_Boolean(ptr %self, i1 %other) {
entry:
  %selfBoolPtr = getelementptr inbounds %Boolean, ptr %self, i32 0, i32 0
  %selfValue = load i1, ptr %selfBoolPtr, align 1
  %xorResult = xor i1 %selfValue, %other
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %xorResult)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define void @Boolean_print(ptr %0) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  %loadBool = load i1, ptr %boolFieldPtr, align 1
  br i1 %loadBool, label %print_true, label %print_false

print_true:                                       ; preds = %entry
  %true_ptr = getelementptr inbounds [5 x i8], ptr @true_str, i32 0, i32 0
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, ptr %true_ptr)
  ret void

print_false:                                      ; preds = %entry
  %false_ptr = getelementptr inbounds [6 x i8], ptr @false_str, i32 0, i32 0
  %2 = call i32 (ptr, ...) @printf(ptr @fmt, ptr %false_ptr)
  ret void
}

declare i32 @printf(ptr, ...)

define void @Integer_Constructor_Integer(ptr %0, i32 %1) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}

define void @Integer_Constructor(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 0, ptr %intFieldPtr, align 4
  ret void
}

define void @Integer_Constructor_Real(ptr %0, double %1) {
entry:
  %intValue = fptosi double %1 to i32
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %intValue, ptr %intFieldPtr, align 4
  ret void
}

define i32 @Integer_get(ptr %integerPtr) {
entry:
  %fieldPtr = getelementptr inbounds %Integer, ptr %integerPtr, i32 0, i32 0
  %value = load i32, ptr %fieldPtr, align 4
  ret i32 %value
}

define %Integer @Integer_Min(ptr %self) {
entry:
  %minValue = insertvalue %Integer undef, i32 -2147483648, 0
  ret %Integer %minValue
}

define %Integer @Integer_Max(ptr %self) {
entry:
  %maxValue = insertvalue %Integer undef, i32 2147483647, 0
  ret %Integer %maxValue
}

define %Real @Integer_toReal(ptr %self) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %realResult = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %realResult, double %selfAsReal)
  %loadedReal = load %Real, ptr %realResult, align 8
  ret %Real %loadedReal
}

define %Boolean @Integer_toBoolean(ptr %self) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %isNonZero = icmp ne i32 %selfValue, 0
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %isNonZero)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Integer @Integer_UnaryMinus(ptr %self) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %result = sub i32 %selfValue, 1
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Mult_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = mul i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Plus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = add i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Minus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sub i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Div_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sdiv i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Rem_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = srem i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Boolean @Integer_Less_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp slt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_LessEqual_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp sle i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp sgt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_GreaterEqual_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp sge i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Equal_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp eq i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Less_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp olt double %selfAsReal, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_LessEqual_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp ole double %selfAsReal, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp ogt double %selfAsReal, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_GreaterEqual_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp oge double %selfAsReal, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Equal_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp oeq double %selfAsReal, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define void @Integer_print(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %loadInt = load i32, ptr %intFieldPtr, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.1, i32 %loadInt)
  ret void
}

define void @Integer_scan(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %scanResult = call i32 (ptr, ...) @scanf(ptr @fmt_scan, ptr %intFieldPtr)
  %ifcond = icmp eq i32 %scanResult, 1
  br i1 %ifcond, label %validInput, label %invalidInput

validInput:                                       ; preds = %entry
  ret void

invalidInput:                                     ; preds = %entry
  %1 = call i32 (ptr, ...) @printf(ptr @error_msg)
  call void @exit(i32 1)
  ret void
}

declare i32 @scanf(ptr, ...)

declare void @exit(i32)

define void @Real_Constructor_Real(ptr %0, double %1) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  store double %1, ptr %doubleFieldPtr, align 8
  ret void
}

define void @Real_Constructor(ptr %0) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  store double 0.000000e+00, ptr %doubleFieldPtr, align 8
  ret void
}

define void @Real_Constructor_Integer(ptr %realPtr, %Integer %integerValue) {
entry:
  %integerFieldValue = extractvalue %Integer %integerValue, 0
  %realValue = sitofp i32 %integerFieldValue to double
  %realFieldPtr = getelementptr inbounds %Real, ptr %realPtr, i32 0, i32 0
  store double %realValue, ptr %realFieldPtr, align 8
  ret void
}

define %Integer @Real_toInteger(ptr %self) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %convertedValue = fptosi double %selfValue to i32
  %resultPtr = alloca %Integer, align 4
  call void @Integer_Constructor_Integer(ptr %resultPtr, i32 %convertedValue)
  %loadedResult = load %Integer, ptr %resultPtr, align 4
  ret %Integer %loadedResult
}

define %Real @Real_UnaryMinus(ptr %self) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %negatedValue = fsub double %selfValue, 1.000000e+00
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %negatedValue)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Plus_Real(ptr %self, %Real %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Real, align 8
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8
  %result = fadd double %selfValue, %otherValue
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Minus_Real(ptr %self, %Real %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Real, align 8
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8
  %result = fsub double %selfValue, %otherValue
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Mult_Real(ptr %self, %Real %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Real, align 8
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8
  %result = fmul double %selfValue, %otherValue
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Div_Real(ptr %self, %Real %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Real, align 8
  store %Real %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Real, ptr %otherPtr, i32 0, i32 0
  %otherValue = load double, ptr %otherFieldPtr, align 8
  %result = fdiv double %selfValue, %otherValue
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Plus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %otherAsReal = sitofp i32 %otherValue to double
  %result = fadd double %selfValue, %otherAsReal
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Minus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %otherAsReal = sitofp i32 %otherValue to double
  %result = fsub double %selfValue, %otherAsReal
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Mult_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %otherAsReal = sitofp i32 %otherValue to double
  %result = fmul double %selfValue, %otherAsReal
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Div_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %otherAsReal = sitofp i32 %otherValue to double
  %result = fdiv double %selfValue, %otherAsReal
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Rem_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 8
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %otherAsReal = sitofp i32 %otherValue to double
  %result = frem double %selfValue, %otherAsReal
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %result)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Min() {
entry:
  %minValue = fsub double 0.000000e+00, 0x7FEFFFFFFFFFFFFF
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %minValue)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Max() {
entry:
  %maxValue = fadd double 0.000000e+00, 0x7FEFFFFFFFFFFFFF
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %maxValue)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Real @Real_Epsilon() {
entry:
  %epsilonValue = fadd double 0.000000e+00, 0x3CB0000000000000
  %resultPtr = alloca %Real, align 8
  call void @Real_Constructor_Real(ptr %resultPtr, double %epsilonValue)
  %loadedResult = load %Real, ptr %resultPtr, align 8
  ret %Real %loadedResult
}

define %Boolean @Real_Less_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %result = fcmp olt double %selfValue, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Greater_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %result = fcmp ogt double %selfValue, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_LessEqual_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %result = fcmp ole double %selfValue, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_GreaterEqual_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %result = fcmp oge double %selfValue, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Equal_Real(ptr %self, double %realValue) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %result = fcmp oeq double %selfValue, %realValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Less_Integer(ptr %self, i32 %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp olt double %selfValue, %otherAsReal
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_LessEqual_Integer(ptr %self, i32 %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp ole double %selfValue, %otherAsReal
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Greater_Integer(ptr %self, i32 %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp ogt double %selfValue, %otherAsReal
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_GreaterEqual_Integer(ptr %self, i32 %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp oge double %selfValue, %otherAsReal
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Real_Equal_Integer(ptr %self, i32 %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Real, ptr %self, i32 0, i32 0
  %selfValue = load double, ptr %selfFieldPtr, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp oeq double %selfValue, %otherAsReal
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define void @Real_print(ptr %0) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  %loadDouble = load double, ptr %doubleFieldPtr, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.4, double %loadDouble)
  ret void
}

define void @IntArray_Create_Default(ptr %arrayPtr, i32 %length) {
entry:
  %elementSize = mul i32 %length, 4
  %dataPtr = call ptr @malloc(i32 %elementSize)
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  store ptr %dataPtr, ptr %dataFieldPtr, align 8
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %length, ptr %lengthFieldPtr, align 4
  ret void
}

declare ptr @malloc(i32)

define %Integer @IntArray_Length(ptr %arrayPtr) {
entry:
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr, align 4
  %newInteger = alloca %Integer, align 4
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %newInteger, i32 0, i32 0
  store i32 %length, ptr %integerFieldPtr, align 4
  %returnValue = load %Integer, ptr %newInteger, align 4
  ret %Integer %returnValue
}

define %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 %index) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index
  %value = load i32, ptr %elementPtr, align 4
  %integerValue = alloca %Integer, align 8
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %integerValue, i32 0, i32 0
  store i32 %value, ptr %integerFieldPtr, align 4
  %returnValue = load %Integer, ptr %integerValue, align 4
  ret %Integer %returnValue
}

define void @IntArray_set_Integer_Integer(ptr %arrayPtr, i32 %index, i32 %value) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index
  store i32 %value, ptr %elementPtr, align 4
  ret void
}

define void @IntArray_print(ptr %arrayPtr) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr, align 4
  %index = alloca i32, align 4
  store i32 0, ptr %index, align 4
  br label %loop

loop:                                             ; preds = %body, %entry
  %currentIndex = load i32, ptr %index, align 4
  %isEnd = icmp slt i32 %currentIndex, %length
  br i1 %isEnd, label %body, label %end

body:                                             ; preds = %loop
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %currentIndex
  %elementValue = load i32, ptr %elementPtr, align 4
  %0 = call i32 (ptr, ...) @printf(ptr @int_fmt, i32 %elementValue)
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index, align 4
  br label %loop

end:                                              ; preds = %loop
  ret void
}

define void @IntegerTester_Init(ptr %0) {
entry:
  ret void
}

define void @IntegerTester_test(ptr %this) {
entry:
  %mallocCall = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall)
  store i32 1, ptr %mallocCall, align 4
  call void @Integer_print(ptr %mallocCall)
  %mallocCall1 = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Integer(ptr %mallocCall1, i32 10)
  call void @Integer_print(ptr %mallocCall1)
  %mallocCall2 = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Real(ptr %mallocCall2, double 1.450000e+01)
  call void @Integer_print(ptr %mallocCall2)
  %mallocCall3 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall3)
  %call_toReal = call %Real @Integer_toReal(ptr %mallocCall)
  %alloca_return_val = alloca %Real, align 8
  store %Real %call_toReal, ptr %alloca_return_val, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall3, ptr %alloca_return_val, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall3)
  %mallocCall4 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor_Boolean(ptr %mallocCall4, i1 false)
  %call_toBoolean = call %Boolean @Integer_toBoolean(ptr %mallocCall)
  %alloca_return_val5 = alloca %Boolean, align 8
  store %Boolean %call_toBoolean, ptr %alloca_return_val5, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall4, ptr %alloca_return_val5, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall4)
  %mallocCall6 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall6)
  %call_UnaryMinus = call %Integer @Integer_UnaryMinus(ptr %mallocCall)
  %alloca_return_val7 = alloca %Integer, align 8
  store %Integer %call_UnaryMinus, ptr %alloca_return_val7, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall6, ptr %alloca_return_val7, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall6)
  %mallocCall8 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall8)
  %call_Min = call %Integer @Integer_Min(ptr %mallocCall8)
  %alloca_return_val9 = alloca %Integer, align 8
  store %Integer %call_Min, ptr %alloca_return_val9, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall8, ptr %alloca_return_val9, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall8)
  %mallocCall10 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall10)
  %call_Max = call %Integer @Integer_Max(ptr %mallocCall10)
  %alloca_return_val11 = alloca %Integer, align 8
  store %Integer %call_Max, ptr %alloca_return_val11, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall10, ptr %alloca_return_val11, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall10)
  %mallocCall12 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall12)
  %loaded_arg = load %Integer, ptr %mallocCall1, align 4
  %call_Plus = call %Integer @Integer_Plus_Integer(ptr %mallocCall, %Integer %loaded_arg)
  %alloca_return_val13 = alloca %Integer, align 8
  store %Integer %call_Plus, ptr %alloca_return_val13, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall12, ptr %alloca_return_val13, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall12)
  %mallocCall14 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall14)
  %loaded_arg15 = load %Integer, ptr %mallocCall1, align 4
  %call_Minus = call %Integer @Integer_Minus_Integer(ptr %mallocCall, %Integer %loaded_arg15)
  %alloca_return_val16 = alloca %Integer, align 8
  store %Integer %call_Minus, ptr %alloca_return_val16, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall14, ptr %alloca_return_val16, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall14)
  %mallocCall17 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall17)
  %loaded_arg18 = load %Integer, ptr %mallocCall1, align 4
  %call_Mult = call %Integer @Integer_Mult_Integer(ptr %mallocCall, %Integer %loaded_arg18)
  %alloca_return_val19 = alloca %Integer, align 8
  store %Integer %call_Mult, ptr %alloca_return_val19, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall17, ptr %alloca_return_val19, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall17)
  %mallocCall20 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall20)
  %loaded_arg21 = load %Integer, ptr %mallocCall1, align 4
  %call_Div = call %Integer @Integer_Div_Integer(ptr %mallocCall, %Integer %loaded_arg21)
  %alloca_return_val22 = alloca %Integer, align 8
  store %Integer %call_Div, ptr %alloca_return_val22, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall20, ptr %alloca_return_val22, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall20)
  %mallocCall23 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall23)
  %loaded_arg24 = load %Integer, ptr %mallocCall1, align 4
  %call_Less = call %Boolean @Integer_Less_Integer(ptr %mallocCall, %Integer %loaded_arg24)
  %alloca_return_val25 = alloca %Boolean, align 8
  store %Boolean %call_Less, ptr %alloca_return_val25, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall23, ptr %alloca_return_val25, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall23)
  %mallocCall26 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall26)
  %loaded_arg27 = load %Real, ptr %mallocCall3, align 8
  %call_Less28 = call %Boolean @Integer_Less_Real(ptr %mallocCall, %Real %loaded_arg27)
  %alloca_return_val29 = alloca %Boolean, align 8
  store %Boolean %call_Less28, ptr %alloca_return_val29, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall26, ptr %alloca_return_val29, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall26)
  %mallocCall30 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall30)
  %loaded_arg31 = load %Integer, ptr %mallocCall1, align 4
  %call_LessEqual = call %Boolean @Integer_LessEqual_Integer(ptr %mallocCall, %Integer %loaded_arg31)
  %alloca_return_val32 = alloca %Boolean, align 8
  store %Boolean %call_LessEqual, ptr %alloca_return_val32, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall30, ptr %alloca_return_val32, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall30)
  %mallocCall33 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall33)
  %loaded_arg34 = load %Real, ptr %mallocCall3, align 8
  %call_LessEqual35 = call %Boolean @Integer_LessEqual_Real(ptr %mallocCall, %Real %loaded_arg34)
  %alloca_return_val36 = alloca %Boolean, align 8
  store %Boolean %call_LessEqual35, ptr %alloca_return_val36, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall33, ptr %alloca_return_val36, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall33)
  %mallocCall37 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall37)
  %loaded_arg38 = load %Integer, ptr %mallocCall1, align 4
  %call_Greater = call %Boolean @Integer_Greater_Integer(ptr %mallocCall, %Integer %loaded_arg38)
  %alloca_return_val39 = alloca %Boolean, align 8
  store %Boolean %call_Greater, ptr %alloca_return_val39, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall37, ptr %alloca_return_val39, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall37)
  %mallocCall40 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall40)
  %loaded_arg41 = load %Real, ptr %mallocCall3, align 8
  %call_Greater42 = call %Boolean @Integer_Greater_Real(ptr %mallocCall, %Real %loaded_arg41)
  %alloca_return_val43 = alloca %Boolean, align 8
  store %Boolean %call_Greater42, ptr %alloca_return_val43, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall40, ptr %alloca_return_val43, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall40)
  %mallocCall44 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall44)
  %loaded_arg45 = load %Integer, ptr %mallocCall1, align 4
  %call_GreaterEqual = call %Boolean @Integer_GreaterEqual_Integer(ptr %mallocCall, %Integer %loaded_arg45)
  %alloca_return_val46 = alloca %Boolean, align 8
  store %Boolean %call_GreaterEqual, ptr %alloca_return_val46, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall44, ptr %alloca_return_val46, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall44)
  %mallocCall47 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall47)
  %loaded_arg48 = load %Real, ptr %mallocCall3, align 8
  %call_GreaterEqual49 = call %Boolean @Integer_GreaterEqual_Real(ptr %mallocCall, %Real %loaded_arg48)
  %alloca_return_val50 = alloca %Boolean, align 8
  store %Boolean %call_GreaterEqual49, ptr %alloca_return_val50, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall47, ptr %alloca_return_val50, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall47)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

define void @IntegerTester_Constructor(ptr %0) {
entry:
  call void @IntegerTester_Init(ptr %0)
  ret void
}

define void @RealTester_Init(ptr %0) {
entry:
  ret void
}

define void @RealTester_test(ptr %this) {
entry:
  %mallocCall = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall)
  store double 1.100000e+00, ptr %mallocCall, align 8
  call void @Real_print(ptr %mallocCall)
  %mallocCall1 = call ptr @malloc(i64 8)
  call void @Real_Constructor_Real(ptr %mallocCall1, double 1.450000e+01)
  call void @Real_print(ptr %mallocCall1)
  %mallocCall2 = call ptr @malloc(i64 8)
  call void @Real_Constructor_Integer(ptr %mallocCall2, i32 14)
  call void @Real_print(ptr %mallocCall2)
  %mallocCall3 = call ptr @malloc(i64 4)
  call void @Integer_Constructor(ptr %mallocCall3)
  %call_toInteger = call %Integer @Real_toInteger(ptr %mallocCall1)
  %alloca_return_val = alloca %Integer, align 8
  store %Integer %call_toInteger, ptr %alloca_return_val, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall3, ptr %alloca_return_val, i64 8, i1 false)
  call void @Integer_print(ptr %mallocCall3)
  %mallocCall4 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall4)
  %call_UnaryMinus = call %Real @Real_UnaryMinus(ptr %mallocCall1)
  %alloca_return_val5 = alloca %Real, align 8
  store %Real %call_UnaryMinus, ptr %alloca_return_val5, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall4, ptr %alloca_return_val5, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall4)
  %mallocCall6 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall6)
  %loaded_arg = load %Real, ptr %mallocCall, align 8
  %call_Plus = call %Real @Real_Plus_Real(ptr %mallocCall1, %Real %loaded_arg)
  %alloca_return_val7 = alloca %Real, align 8
  store %Real %call_Plus, ptr %alloca_return_val7, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall6, ptr %alloca_return_val7, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall6)
  %mallocCall8 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall8)
  %loaded_arg9 = load %Real, ptr %mallocCall, align 8
  %call_Minus = call %Real @Real_Minus_Real(ptr %mallocCall1, %Real %loaded_arg9)
  %alloca_return_val10 = alloca %Real, align 8
  store %Real %call_Minus, ptr %alloca_return_val10, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall8, ptr %alloca_return_val10, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall8)
  %mallocCall11 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall11)
  %loaded_arg12 = load %Real, ptr %mallocCall, align 8
  %call_Mult = call %Real @Real_Mult_Real(ptr %mallocCall1, %Real %loaded_arg12)
  %alloca_return_val13 = alloca %Real, align 8
  store %Real %call_Mult, ptr %alloca_return_val13, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall11, ptr %alloca_return_val13, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall11)
  %mallocCall14 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall14)
  %loaded_arg15 = load %Real, ptr %mallocCall, align 8
  %call_Div = call %Real @Real_Div_Real(ptr %mallocCall1, %Real %loaded_arg15)
  %alloca_return_val16 = alloca %Real, align 8
  store %Real %call_Div, ptr %alloca_return_val16, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall14, ptr %alloca_return_val16, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall14)
  %mallocCall17 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall17)
  %loaded_arg18 = load %Integer, ptr %mallocCall3, align 4
  %call_Plus19 = call %Real @Real_Plus_Integer(ptr %mallocCall1, %Integer %loaded_arg18)
  %alloca_return_val20 = alloca %Real, align 8
  store %Real %call_Plus19, ptr %alloca_return_val20, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall17, ptr %alloca_return_val20, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall17)
  %mallocCall21 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall21)
  %loaded_arg22 = load %Integer, ptr %mallocCall3, align 4
  %call_Minus23 = call %Real @Real_Minus_Integer(ptr %mallocCall1, %Integer %loaded_arg22)
  %alloca_return_val24 = alloca %Real, align 8
  store %Real %call_Minus23, ptr %alloca_return_val24, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall21, ptr %alloca_return_val24, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall21)
  %mallocCall25 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall25)
  %loaded_arg26 = load %Integer, ptr %mallocCall3, align 4
  %call_Mult27 = call %Real @Real_Mult_Integer(ptr %mallocCall1, %Integer %loaded_arg26)
  %alloca_return_val28 = alloca %Real, align 8
  store %Real %call_Mult27, ptr %alloca_return_val28, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall25, ptr %alloca_return_val28, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall25)
  %mallocCall29 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall29)
  %loaded_arg30 = load %Integer, ptr %mallocCall3, align 4
  %call_Div31 = call %Real @Real_Div_Integer(ptr %mallocCall1, %Integer %loaded_arg30)
  %alloca_return_val32 = alloca %Real, align 8
  store %Real %call_Div31, ptr %alloca_return_val32, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall29, ptr %alloca_return_val32, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall29)
  %mallocCall33 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall33)
  %loaded_arg34 = load %Integer, ptr %mallocCall3, align 4
  %call_Rem = call %Real @Real_Rem_Integer(ptr %mallocCall1, %Integer %loaded_arg34)
  %alloca_return_val35 = alloca %Real, align 8
  store %Real %call_Rem, ptr %alloca_return_val35, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall33, ptr %alloca_return_val35, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall33)
  %mallocCall36 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall36)
  %call_Max = call %Real @Real_Max(ptr %mallocCall36)
  %alloca_return_val37 = alloca %Real, align 8
  store %Real %call_Max, ptr %alloca_return_val37, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall36, ptr %alloca_return_val37, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall36)
  %mallocCall38 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall38)
  %call_Min = call %Real @Real_Min(ptr %mallocCall38)
  %alloca_return_val39 = alloca %Real, align 8
  store %Real %call_Min, ptr %alloca_return_val39, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall38, ptr %alloca_return_val39, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall38)
  %mallocCall40 = call ptr @malloc(i64 8)
  call void @Real_Constructor(ptr %mallocCall40)
  %call_Epsilon = call %Real @Real_Epsilon(ptr %mallocCall40)
  %alloca_return_val41 = alloca %Real, align 8
  store %Real %call_Epsilon, ptr %alloca_return_val41, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall40, ptr %alloca_return_val41, i64 8, i1 false)
  call void @Real_print(ptr %mallocCall40)
  %mallocCall42 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall42)
  %loaded_arg43 = load %Real, ptr %mallocCall, align 8
  %call_Less = call %Boolean @Real_Less_Real(ptr %mallocCall1, %Real %loaded_arg43)
  %alloca_return_val44 = alloca %Boolean, align 8
  store %Boolean %call_Less, ptr %alloca_return_val44, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall42, ptr %alloca_return_val44, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall42)
  %mallocCall45 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall45)
  %loaded_arg46 = load %Real, ptr %mallocCall, align 8
  %call_LessEqual = call %Boolean @Real_LessEqual_Real(ptr %mallocCall1, %Real %loaded_arg46)
  %alloca_return_val47 = alloca %Boolean, align 8
  store %Boolean %call_LessEqual, ptr %alloca_return_val47, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall45, ptr %alloca_return_val47, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall45)
  %mallocCall48 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall48)
  %loaded_arg49 = load %Real, ptr %mallocCall, align 8
  %call_Greater = call %Boolean @Real_Greater_Real(ptr %mallocCall1, %Real %loaded_arg49)
  %alloca_return_val50 = alloca %Boolean, align 8
  store %Boolean %call_Greater, ptr %alloca_return_val50, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall48, ptr %alloca_return_val50, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall48)
  %mallocCall51 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall51)
  %loaded_arg52 = load %Real, ptr %mallocCall, align 8
  %call_GreaterEqual = call %Boolean @Real_GreaterEqual_Real(ptr %mallocCall1, %Real %loaded_arg52)
  %alloca_return_val53 = alloca %Boolean, align 8
  store %Boolean %call_GreaterEqual, ptr %alloca_return_val53, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall51, ptr %alloca_return_val53, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall51)
  %mallocCall54 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall54)
  %loaded_arg55 = load %Real, ptr %mallocCall, align 8
  %call_Equal = call %Boolean @Real_Equal_Real(ptr %mallocCall1, %Real %loaded_arg55)
  %alloca_return_val56 = alloca %Boolean, align 8
  store %Boolean %call_Equal, ptr %alloca_return_val56, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall54, ptr %alloca_return_val56, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall54)
  %mallocCall57 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall57)
  %loaded_arg58 = load %Integer, ptr %mallocCall3, align 4
  %call_Less59 = call %Boolean @Real_Less_Integer(ptr %mallocCall1, %Integer %loaded_arg58)
  %alloca_return_val60 = alloca %Boolean, align 8
  store %Boolean %call_Less59, ptr %alloca_return_val60, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall57, ptr %alloca_return_val60, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall57)
  %mallocCall61 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall61)
  %loaded_arg62 = load %Integer, ptr %mallocCall3, align 4
  %call_LessEqual63 = call %Boolean @Real_LessEqual_Integer(ptr %mallocCall1, %Integer %loaded_arg62)
  %alloca_return_val64 = alloca %Boolean, align 8
  store %Boolean %call_LessEqual63, ptr %alloca_return_val64, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall61, ptr %alloca_return_val64, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall61)
  %mallocCall65 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall65)
  %loaded_arg66 = load %Integer, ptr %mallocCall3, align 4
  %call_Greater67 = call %Boolean @Real_Greater_Integer(ptr %mallocCall1, %Integer %loaded_arg66)
  %alloca_return_val68 = alloca %Boolean, align 8
  store %Boolean %call_Greater67, ptr %alloca_return_val68, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall65, ptr %alloca_return_val68, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall65)
  %mallocCall69 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall69)
  %loaded_arg70 = load %Integer, ptr %mallocCall3, align 4
  %call_GreaterEqual71 = call %Boolean @Real_GreaterEqual_Integer(ptr %mallocCall1, %Integer %loaded_arg70)
  %alloca_return_val72 = alloca %Boolean, align 8
  store %Boolean %call_GreaterEqual71, ptr %alloca_return_val72, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall69, ptr %alloca_return_val72, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall69)
  %mallocCall73 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall73)
  %loaded_arg74 = load %Integer, ptr %mallocCall3, align 4
  %call_Equal75 = call %Boolean @Real_Equal_Integer(ptr %mallocCall1, %Integer %loaded_arg74)
  %alloca_return_val76 = alloca %Boolean, align 8
  store %Boolean %call_Equal75, ptr %alloca_return_val76, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall73, ptr %alloca_return_val76, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall73)
  ret void
}

define void @RealTester_Constructor(ptr %0) {
entry:
  call void @RealTester_Init(ptr %0)
  ret void
}

define void @BooleanTester_Init(ptr %0) {
entry:
  ret void
}

define void @BooleanTester_test(ptr %this) {
entry:
  %mallocCall = call ptr @malloc(i64 1)
  call void @Boolean_Constructor_Boolean(ptr %mallocCall, i1 true)
  call void @Boolean_print(ptr %mallocCall)
  %mallocCall1 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor_Boolean(ptr %mallocCall1, i1 false)
  call void @Boolean_print(ptr %mallocCall1)
  %mallocCall2 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall2)
  %loaded_arg = load %Boolean, ptr %mallocCall, align 1
  %call_Or = call %Boolean @Boolean_Or_Boolean(ptr %mallocCall1, %Boolean %loaded_arg)
  %alloca_return_val = alloca %Boolean, align 8
  store %Boolean %call_Or, ptr %alloca_return_val, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall2, ptr %alloca_return_val, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall2)
  %mallocCall3 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall3)
  %loaded_arg4 = load %Boolean, ptr %mallocCall, align 1
  %call_And = call %Boolean @Boolean_And_Boolean(ptr %mallocCall1, %Boolean %loaded_arg4)
  %alloca_return_val5 = alloca %Boolean, align 8
  store %Boolean %call_And, ptr %alloca_return_val5, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall3, ptr %alloca_return_val5, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall3)
  %mallocCall6 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall6)
  %loaded_arg7 = load %Boolean, ptr %mallocCall, align 1
  %call_Xor = call %Boolean @Boolean_Xor_Boolean(ptr %mallocCall1, %Boolean %loaded_arg7)
  %alloca_return_val8 = alloca %Boolean, align 8
  store %Boolean %call_Xor, ptr %alloca_return_val8, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall6, ptr %alloca_return_val8, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall6)
  %mallocCall9 = call ptr @malloc(i64 1)
  call void @Boolean_Constructor(ptr %mallocCall9)
  %call_Not = call %Boolean @Boolean_Not(ptr %mallocCall1)
  %alloca_return_val10 = alloca %Boolean, align 8
  store %Boolean %call_Not, ptr %alloca_return_val10, align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %mallocCall9, ptr %alloca_return_val10, i64 8, i1 false)
  call void @Boolean_print(ptr %mallocCall9)
  ret void
}

define void @BooleanTester_Constructor(ptr %0) {
entry:
  call void @BooleanTester_Init(ptr %0)
  ret void
}

define void @Main_Init(ptr %0) {
entry:
  ret void
}

define void @Main_Constructor(ptr %this) {
entry:
  call void @Main_Init(ptr %this)
  %mallocCall = call ptr @malloc(i64 0)
  call void @BooleanTester_Constructor(ptr %mallocCall)
  %mallocCall1 = call ptr @malloc(i64 0)
  call void @BooleanTester_Constructor(ptr %mallocCall1)
  call void @BooleanTester_test(ptr %mallocCall1)
  ret void
}

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Constructor(ptr %main_object)
  ret i32 0
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
