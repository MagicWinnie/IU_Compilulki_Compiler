; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }
%IntArray = type { ptr, i32 }
%IntList = type { %IntArray }
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

define void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 %length) {
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

define void @IntList_Constructor(ptr %listPtr) {
entry:
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0
  call void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 0)
  ret void
}

define void @IntList_Constructor_Integer(ptr %listPtr, i32 %length) {
entry:
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0
  call void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 %length)
  ret void
}

define void @IntList_append_Integer(ptr %listPtr, i32 %value) {
entry:
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0
  %lengthTemp = call %Integer @IntArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Integer, align 8
  store %Integer %lengthTemp, ptr %lengthPtr, align 4
  %lengthFieldPtr = getelementptr inbounds %Integer, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr, align 4
  call void @IntArray_set_Integer_Integer(ptr %arrayPtr, i32 %length, i32 %value)
  %newLength = add i32 %length, 1
  %lengthFieldPtr2 = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %newLength, ptr %lengthFieldPtr2, align 4
  ret void
}

define %Integer @IntList_head(ptr %listPtr) {
entry:
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0
  %value = call %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 0)
  ret %Integer %value
}

define ptr @IntList_tail(ptr %listPtr) {
entry:
  %nullPtr = inttoptr i32 0 to ptr
  %endPtr = getelementptr inbounds %IntList, ptr %nullPtr, i32 1
  %listSize = ptrtoint ptr %endPtr to i32
  %newListPtr = call ptr @malloc(i32 %listSize)
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0
  %lengthTemp = call %Integer @IntArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Integer, align 8
  store %Integer %lengthTemp, ptr %lengthPtr, align 4
  %lengthFieldPtr = getelementptr inbounds %Integer, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr, align 4
  %newLength = sub i32 %length, 1
  %newArrayPtr = getelementptr inbounds %IntList, ptr %newListPtr, i32 0, i32 0
  call void @IntArray_Constructor_Integer(ptr %newArrayPtr, i32 %newLength)
  %index = alloca i32, align 4
  store i32 0, ptr %index, align 4
  br label %loop

loop:                                             ; preds = %body, %entry
  %currentIndex = load i32, ptr %index, align 4
  %isEnd = icmp slt i32 %currentIndex, %newLength
  br i1 %isEnd, label %body, label %end

body:                                             ; preds = %loop
  %sourceIndex = add i32 %currentIndex, 1
  %valueTemp = call %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 %sourceIndex)
  %valuePtr = alloca %Integer, align 8
  store %Integer %valueTemp, ptr %valuePtr, align 4
  %valueFieldPtr = getelementptr inbounds %Integer, ptr %valuePtr, i32 0, i32 0
  %valueInt = load i32, ptr %valueFieldPtr, align 4
  call void @IntArray_set_Integer_Integer(ptr %newArrayPtr, i32 %currentIndex, i32 %valueInt)
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index, align 4
  br label %loop

end:                                              ; preds = %loop
  ret ptr %newListPtr
}

define void @Test_Init(ptr %0) {
entry:
  ret void
}

define %Integer @Test_print(ptr %this) {
entry:
  %mallocCall = call ptr @malloc(i64 1)
  call void @Boolean_Constructor_Boolean(ptr %mallocCall, i1 true)
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %mallocCall, i32 0, i32 0
  %loadBoolValue = load i1, ptr %boolFieldPtr, align 1
  %ifcond = icmp ne i1 %loadBoolValue, false
  br i1 %ifcond, label %btrue, label %bfalse

btrue:                                            ; preds = %entry
  %mallocCall1 = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Integer(ptr %mallocCall1, i32 0)
  %returnVal = load %Integer, ptr %mallocCall1, align 4
  ret %Integer %returnVal
  br label %end

bfalse:                                           ; preds = %entry
  %mallocCall2 = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Integer(ptr %mallocCall2, i32 1)
  %returnVal3 = load %Integer, ptr %mallocCall2, align 4
  ret %Integer %returnVal3
  br label %end

end:                                              ; preds = %bfalse, %btrue
}

define void @Test_Constructor(ptr %0) {
entry:
  call void @Test_Init(ptr %0)
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
  call void @Test_Constructor(ptr %mallocCall)
  %call_print = call %Integer @Test_print(ptr %mallocCall)
  %alloca_return_val = alloca %Integer, align 8
  store %Integer %call_print, ptr %alloca_return_val, align 4
  call void @Integer_print(ptr %alloca_return_val)
  ret void
}

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Constructor(ptr %main_object)
  ret i32 0
}
