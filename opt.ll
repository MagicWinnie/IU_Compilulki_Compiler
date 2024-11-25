; ModuleID = 'output.ll'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }

@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.4 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@int_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str = private unnamed_addr constant [32 x i8] c"Invalid input. Exiting program.\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @Boolean_Init(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Boolean_Constructor_Boolean(ptr nocapture writeonly %0, i1 %1) local_unnamed_addr #1 {
entry:
  store i1 %1, ptr %0, align 1
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Boolean_Constructor(ptr nocapture writeonly %0) local_unnamed_addr #1 {
entry:
  store i1 false, ptr %0, align 1
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Boolean_Or_Boolean(ptr nocapture readonly %self, %Boolean %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i1, ptr %self, align 1
  %otherFieldValue = extractvalue %Boolean %other, 0
  %orResult = or i1 %otherFieldValue, %selfValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %orResult, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Boolean_And_Boolean(ptr nocapture readonly %self, i1 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i1, ptr %self, align 1
  %andResult = and i1 %selfValue, %other
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %andResult, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Boolean_Not(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load i1, ptr %self, align 1
  %notResult = xor i1 %selfValue, true
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %notResult, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Boolean_Xor_Boolean(ptr nocapture readonly %self, i1 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i1, ptr %self, align 1
  %xorResult = xor i1 %selfValue, %other
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %xorResult, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: nofree nounwind
define void @Boolean_print(ptr nocapture readonly %0) local_unnamed_addr #3 {
entry:
  %loadBool = load i1, ptr %0, align 1
  %spec.select = select i1 %loadBool, ptr @true_str, ptr @false_str
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) %spec.select)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @Integer_Init(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Integer_Constructor_Integer(ptr nocapture writeonly %0, i32 %1) local_unnamed_addr #1 {
entry:
  store i32 %1, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Integer_Constructor(ptr nocapture writeonly %0) local_unnamed_addr #1 {
entry:
  store i32 0, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Integer_Constructor_Real(ptr nocapture writeonly %0, double %1) local_unnamed_addr #1 {
entry:
  %intValue = fptosi double %1 to i32
  store i32 %intValue, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define i32 @Integer_get(ptr nocapture readonly %integerPtr) local_unnamed_addr #2 {
entry:
  %value = load i32, ptr %integerPtr, align 4
  ret i32 %value
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Integer @Integer_Min(ptr nocapture readnone %self) local_unnamed_addr #0 {
entry:
  ret %Integer { i32 -2147483648 }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Integer @Integer_Max(ptr nocapture readnone %self) local_unnamed_addr #0 {
entry:
  ret %Integer { i32 2147483647 }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Integer_toReal(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %loadedReal.fca.0.insert = insertvalue %Real poison, double %selfAsReal, 0
  ret %Real %loadedReal.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_toBoolean(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %isNonZero = icmp ne i32 %selfValue, 0
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %isNonZero, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_UnaryMinus(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %result = add i32 %selfValue, -1
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_Mult_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = mul i32 %selfValue, %other.fca.0.extract
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_Plus_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = add i32 %selfValue, %other.fca.0.extract
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_Minus_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = sub i32 %selfValue, %other.fca.0.extract
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_Div_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = sdiv i32 %selfValue, %other.fca.0.extract
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Integer_Rem_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = srem i32 %selfValue, %other.fca.0.extract
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Less_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = icmp slt i32 %selfValue, %other.fca.0.extract
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_LessEqual_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = icmp sle i32 %selfValue, %other.fca.0.extract
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Greater_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = icmp sgt i32 %selfValue, %other.fca.0.extract
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_GreaterEqual_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = icmp sge i32 %selfValue, %other.fca.0.extract
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Equal_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %result = icmp eq i32 %selfValue, %other.fca.0.extract
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Less_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp olt double %selfAsReal, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_LessEqual_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp ole double %selfAsReal, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Greater_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp ogt double %selfAsReal, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_GreaterEqual_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp oge double %selfAsReal, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Integer_Equal_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load i32, ptr %self, align 4
  %selfAsReal = sitofp i32 %selfValue to double
  %result = fcmp oeq double %selfAsReal, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: nofree nounwind
define void @Integer_print(ptr nocapture readonly %0) local_unnamed_addr #3 {
entry:
  %loadInt = load i32, ptr %0, align 4
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @fmt.1, i32 %loadInt)
  ret void
}

define void @Integer_scan(ptr %0) local_unnamed_addr {
entry:
  %scanResult = tail call i32 (ptr, ...) @scanf(ptr nonnull @fmt_scan, ptr %0)
  %ifcond = icmp eq i32 %scanResult, 1
  br i1 %ifcond, label %common.ret, label %invalidInput

common.ret:                                       ; preds = %entry, %invalidInput
  ret void

invalidInput:                                     ; preds = %entry
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @exit(i32 1)
  br label %common.ret
}

; Function Attrs: nofree nounwind
declare noundef i32 @scanf(ptr nocapture noundef readonly, ...) local_unnamed_addr #3

declare void @exit(i32) local_unnamed_addr

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @Real_Init(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Real_Constructor_Real(ptr nocapture writeonly %0, double %1) local_unnamed_addr #1 {
entry:
  store double %1, ptr %0, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Real_Constructor(ptr nocapture writeonly %0) local_unnamed_addr #1 {
entry:
  store double 0.000000e+00, ptr %0, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @Real_Constructor_Integer(ptr nocapture writeonly %realPtr, %Integer %integerValue) local_unnamed_addr #1 {
entry:
  %integerFieldValue = extractvalue %Integer %integerValue, 0
  %realValue = sitofp i32 %integerFieldValue to double
  store double %realValue, ptr %realPtr, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @Real_toInteger(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %convertedValue = fptosi double %selfValue to i32
  %loadedResult.fca.0.insert = insertvalue %Integer poison, i32 %convertedValue, 0
  ret %Integer %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_UnaryMinus(ptr nocapture readonly %self) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %negatedValue = fadd double %selfValue, -1.000000e+00
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %negatedValue, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Plus_Real(ptr nocapture readonly %self, %Real %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Real %other, 0
  %result = fadd double %other.fca.0.extract, %selfValue
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Minus_Real(ptr nocapture readonly %self, %Real %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Real %other, 0
  %result = fsub double %selfValue, %other.fca.0.extract
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Mult_Real(ptr nocapture readonly %self, %Real %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Real %other, 0
  %result = fmul double %other.fca.0.extract, %selfValue
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Div_Real(ptr nocapture readonly %self, %Real %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Real %other, 0
  %result = fdiv double %selfValue, %other.fca.0.extract
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Plus_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %otherAsReal = sitofp i32 %other.fca.0.extract to double
  %result = fadd double %selfValue, %otherAsReal
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Minus_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %otherAsReal = sitofp i32 %other.fca.0.extract to double
  %result = fsub double %selfValue, %otherAsReal
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Mult_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %otherAsReal = sitofp i32 %other.fca.0.extract to double
  %result = fmul double %selfValue, %otherAsReal
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Div_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %otherAsReal = sitofp i32 %other.fca.0.extract to double
  %result = fdiv double %selfValue, %otherAsReal
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @Real_Rem_Integer(ptr nocapture readonly %self, %Integer %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %other.fca.0.extract = extractvalue %Integer %other, 0
  %otherAsReal = sitofp i32 %other.fca.0.extract to double
  %result = frem double %selfValue, %otherAsReal
  %loadedResult.fca.0.insert = insertvalue %Real poison, double %result, 0
  ret %Real %loadedResult.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Real @Real_Min() local_unnamed_addr #0 {
entry:
  ret %Real { double 0xFFEFFFFFFFFFFFFF }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Real @Real_Max() local_unnamed_addr #0 {
entry:
  ret %Real { double 0x7FEFFFFFFFFFFFFF }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Real @Real_Epsilon() local_unnamed_addr #0 {
entry:
  ret %Real { double 0x3CB0000000000000 }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Less_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %result = fcmp olt double %selfValue, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Greater_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %result = fcmp ogt double %selfValue, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_LessEqual_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %result = fcmp ole double %selfValue, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_GreaterEqual_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %result = fcmp oge double %selfValue, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Equal_Real(ptr nocapture readonly %self, double %realValue) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %result = fcmp oeq double %selfValue, %realValue
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Less_Integer(ptr nocapture readonly %self, i32 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp olt double %selfValue, %otherAsReal
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_LessEqual_Integer(ptr nocapture readonly %self, i32 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp ole double %selfValue, %otherAsReal
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Greater_Integer(ptr nocapture readonly %self, i32 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp ogt double %selfValue, %otherAsReal
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_GreaterEqual_Integer(ptr nocapture readonly %self, i32 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp oge double %selfValue, %otherAsReal
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @Real_Equal_Integer(ptr nocapture readonly %self, i32 %other) local_unnamed_addr #2 {
entry:
  %selfValue = load double, ptr %self, align 8
  %otherAsReal = sitofp i32 %other to double
  %result = fcmp oeq double %selfValue, %otherAsReal
  %loadedBoolean.fca.0.insert = insertvalue %Boolean poison, i1 %result, 0
  ret %Boolean %loadedBoolean.fca.0.insert
}

; Function Attrs: nofree nounwind
define void @Real_print(ptr nocapture readonly %0) local_unnamed_addr #3 {
entry:
  %loadDouble = load double, ptr %0, align 8
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @fmt.4, double %loadDouble)
  ret void
}

define void @IntArray_Constructor_Integer(ptr nocapture writeonly %arrayPtr, i32 %length) local_unnamed_addr {
entry:
  %elementSize = shl i32 %length, 2
  %dataPtr = tail call ptr @malloc(i32 %elementSize)
  store ptr %dataPtr, ptr %arrayPtr, align 8
  %lengthFieldPtr = getelementptr inbounds i8, ptr %arrayPtr, i64 8
  store i32 %length, ptr %lengthFieldPtr, align 4
  ret void
}

declare ptr @malloc(i32) local_unnamed_addr

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @IntArray_Length(ptr nocapture readonly %arrayPtr) local_unnamed_addr #2 {
entry:
  %lengthFieldPtr = getelementptr inbounds i8, ptr %arrayPtr, i64 8
  %length = load i32, ptr %lengthFieldPtr, align 4
  %returnValue.fca.0.insert = insertvalue %Integer poison, i32 %length, 0
  ret %Integer %returnValue.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none)
define %Integer @IntArray_get_Integer(ptr nocapture readonly %arrayPtr, i32 %index) local_unnamed_addr #4 {
entry:
  %dataPtr = load ptr, ptr %arrayPtr, align 8
  %0 = sext i32 %index to i64
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i64 %0
  %value = load i32, ptr %elementPtr, align 4
  %returnValue.fca.0.insert = insertvalue %Integer poison, i32 %value, 0
  ret %Integer %returnValue.fca.0.insert
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
define void @IntArray_set_Integer_Integer(ptr nocapture readonly %arrayPtr, i32 %index, i32 %value) local_unnamed_addr #5 {
entry:
  %dataPtr = load ptr, ptr %arrayPtr, align 8
  %0 = sext i32 %index to i64
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i64 %0
  store i32 %value, ptr %elementPtr, align 4
  ret void
}

; Function Attrs: nofree nounwind
define void @IntArray_print(ptr nocapture readonly %arrayPtr) local_unnamed_addr #3 {
entry:
  %dataPtr = load ptr, ptr %arrayPtr, align 8
  %lengthFieldPtr = getelementptr inbounds i8, ptr %arrayPtr, i64 8
  %length = load i32, ptr %lengthFieldPtr, align 4
  %isEnd1 = icmp sgt i32 %length, 0
  br i1 %isEnd1, label %body, label %end

body:                                             ; preds = %entry, %body
  %index.02 = phi i32 [ %nextIndex, %body ], [ 0, %entry ]
  %0 = zext nneg i32 %index.02 to i64
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i64 %0
  %elementValue = load i32, ptr %elementPtr, align 4
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @int_fmt, i32 %elementValue)
  %nextIndex = add nuw nsw i32 %index.02, 1
  %isEnd = icmp slt i32 %nextIndex, %length
  br i1 %isEnd, label %body, label %end

end:                                              ; preds = %body, %entry
  ret void
}

define void @IntList_Constructor(ptr nocapture writeonly %listPtr) local_unnamed_addr {
entry:
  %dataPtr.i = tail call ptr @malloc(i32 0)
  store ptr %dataPtr.i, ptr %listPtr, align 8
  %lengthFieldPtr.i = getelementptr inbounds i8, ptr %listPtr, i64 8
  store i32 0, ptr %lengthFieldPtr.i, align 4
  ret void
}

define void @IntList_Constructor_Integer(ptr nocapture writeonly %listPtr, i32 %length) local_unnamed_addr {
entry:
  %elementSize.i = shl i32 %length, 2
  %dataPtr.i = tail call ptr @malloc(i32 %elementSize.i)
  store ptr %dataPtr.i, ptr %listPtr, align 8
  %lengthFieldPtr.i = getelementptr inbounds i8, ptr %listPtr, i64 8
  store i32 %length, ptr %lengthFieldPtr.i, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none)
define void @IntList_append_Integer(ptr nocapture %listPtr, i32 %value) local_unnamed_addr #5 {
entry:
  %lengthFieldPtr.i = getelementptr inbounds i8, ptr %listPtr, i64 8
  %length.i = load i32, ptr %lengthFieldPtr.i, align 4
  %dataPtr.i = load ptr, ptr %listPtr, align 8
  %0 = sext i32 %length.i to i64
  %elementPtr.i = getelementptr inbounds i32, ptr %dataPtr.i, i64 %0
  store i32 %value, ptr %elementPtr.i, align 4
  %newLength = add i32 %length.i, 1
  store i32 %newLength, ptr %lengthFieldPtr.i, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none)
define %Integer @IntList_head(ptr nocapture readonly %listPtr) local_unnamed_addr #4 {
entry:
  %dataPtr.i = load ptr, ptr %listPtr, align 8
  %value.i = load i32, ptr %dataPtr.i, align 4
  %returnValue.fca.0.insert.i = insertvalue %Integer poison, i32 %value.i, 0
  ret %Integer %returnValue.fca.0.insert.i
}

define ptr @IntList_tail(ptr nocapture readonly %listPtr) local_unnamed_addr {
entry:
  %newListPtr = tail call ptr @malloc(i32 16)
  %lengthFieldPtr.i = getelementptr inbounds i8, ptr %listPtr, i64 8
  %length.i = load i32, ptr %lengthFieldPtr.i, align 4
  %newLength = add i32 %length.i, -1
  %elementSize.i = shl i32 %newLength, 2
  %dataPtr.i = tail call ptr @malloc(i32 %elementSize.i)
  store ptr %dataPtr.i, ptr %newListPtr, align 8
  %lengthFieldPtr.i1 = getelementptr inbounds i8, ptr %newListPtr, i64 8
  store i32 %newLength, ptr %lengthFieldPtr.i1, align 4
  %isEnd6 = icmp sgt i32 %newLength, 0
  br i1 %isEnd6, label %body, label %end

body:                                             ; preds = %entry, %body
  %index.07 = phi i32 [ %sourceIndex, %body ], [ 0, %entry ]
  %sourceIndex = add nuw nsw i32 %index.07, 1
  %dataPtr.i2 = load ptr, ptr %listPtr, align 8
  %0 = zext nneg i32 %sourceIndex to i64
  %elementPtr.i = getelementptr inbounds i32, ptr %dataPtr.i2, i64 %0
  %value.i = load i32, ptr %elementPtr.i, align 4
  %dataPtr.i4 = load ptr, ptr %newListPtr, align 8
  %1 = zext nneg i32 %index.07 to i64
  %elementPtr.i5 = getelementptr inbounds i32, ptr %dataPtr.i4, i64 %1
  store i32 %value.i, ptr %elementPtr.i5, align 4
  %isEnd = icmp slt i32 %sourceIndex, %newLength
  br i1 %isEnd, label %body, label %end

end:                                              ; preds = %body, %entry
  ret ptr %newListPtr
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @DerivedInteger_Init(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret void
}

define void @DerivedInteger_print(ptr nocapture readnone %this) local_unnamed_addr {
entry:
  %mallocCall = tail call ptr @malloc(i64 4)
  store i32 5, ptr %mallocCall, align 4
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @fmt.1, i32 5)
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @DerivedInteger_Constructor(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @DerivedInteger_Constructor_Integer(ptr nocapture writeonly %0, i32 %1) local_unnamed_addr #1 {
entry:
  store i32 %1, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Equal_Real(ptr nocapture readonly %0, double %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %result.i = fcmp oeq double %selfAsReal.i, %1
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_toBoolean(ptr nocapture readonly %0) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %isNonZero.i = icmp ne i32 %selfValue.i, 0
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %isNonZero.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_Mult_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = mul i32 %selfValue.i, %other.fca.0.extract.i
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Integer @DerivedInteger_Min(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret %Integer { i32 -2147483648 }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_Div_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = sdiv i32 %selfValue.i, %other.fca.0.extract.i
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define %Integer @DerivedInteger_Max(ptr nocapture readnone %0) local_unnamed_addr #0 {
entry:
  ret %Integer { i32 2147483647 }
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define void @DerivedInteger_Constructor.5(ptr nocapture writeonly %0) local_unnamed_addr #1 {
entry:
  store i32 0, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Less_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = icmp slt i32 %selfValue.i, %other.fca.0.extract.i
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Greater_Real(ptr nocapture readonly %0, double %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %result.i = fcmp ogt double %selfAsReal.i, %1
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_Plus_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = add i32 %selfValue.i, %other.fca.0.extract.i
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_Minus_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = sub i32 %selfValue.i, %other.fca.0.extract.i
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_Rem_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = srem i32 %selfValue.i, %other.fca.0.extract.i
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_GreaterEqual_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = icmp sge i32 %selfValue.i, %other.fca.0.extract.i
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Real @DerivedInteger_toReal(ptr nocapture readonly %0) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %loadedReal.fca.0.insert.i = insertvalue %Real poison, double %selfAsReal.i, 0
  ret %Real %loadedReal.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Less_Real(ptr nocapture readonly %0, double %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %result.i = fcmp olt double %selfAsReal.i, %1
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_LessEqual_Real(ptr nocapture readonly %0, double %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %result.i = fcmp ole double %selfAsReal.i, %1
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_LessEqual_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = icmp sle i32 %selfValue.i, %other.fca.0.extract.i
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_GreaterEqual_Real(ptr nocapture readonly %0, double %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %selfAsReal.i = sitofp i32 %selfValue.i to double
  %result.i = fcmp oge double %selfAsReal.i, %1
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Greater_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = icmp sgt i32 %selfValue.i, %other.fca.0.extract.i
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Boolean @DerivedInteger_Equal_Integer(ptr nocapture readonly %0, %Integer %1) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %other.fca.0.extract.i = extractvalue %Integer %1, 0
  %result.i = icmp eq i32 %selfValue.i, %other.fca.0.extract.i
  %loadedBoolean.fca.0.insert.i = insertvalue %Boolean poison, i1 %result.i, 0
  ret %Boolean %loadedBoolean.fca.0.insert.i
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define %Integer @DerivedInteger_UnaryMinus(ptr nocapture readonly %0) local_unnamed_addr #2 {
entry:
  %selfValue.i = load i32, ptr %0, align 4
  %result.i = add i32 %selfValue.i, -1
  %returnValue.i = insertvalue %Integer undef, i32 %result.i, 0
  ret %Integer %returnValue.i
}

define void @DerivedInteger_scan(ptr %0) local_unnamed_addr {
entry:
  %scanResult.i = tail call i32 (ptr, ...) @scanf(ptr nonnull @fmt_scan, ptr %0)
  %ifcond.i = icmp eq i32 %scanResult.i, 1
  br i1 %ifcond.i, label %Integer_scan.exit, label %invalidInput.i

invalidInput.i:                                   ; preds = %entry
  %puts.i = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @exit(i32 1)
  br label %Integer_scan.exit

Integer_scan.exit:                                ; preds = %entry, %invalidInput.i
  ret void
}

define void @Main_Init(ptr nocapture writeonly %0) local_unnamed_addr {
entry:
  %mallocCall = tail call ptr @malloc(i64 4)
  store i32 0, ptr %mallocCall, align 4
  store ptr %mallocCall, ptr %0, align 8
  ret void
}

define void @Main_Constructor(ptr nocapture writeonly %this) local_unnamed_addr {
entry:
  %mallocCall.i = tail call ptr @malloc(i64 4)
  store i32 0, ptr %mallocCall.i, align 4
  store ptr %mallocCall.i, ptr %this, align 8
  %mallocCall = tail call ptr @malloc(i64 0)
  %mallocCall.i1 = tail call ptr @malloc(i64 4)
  store i32 5, ptr %mallocCall.i1, align 4
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @fmt.1, i32 5)
  ret void
}

define noundef i32 @main() local_unnamed_addr {
entry:
  %mallocCall.i.i = tail call ptr @malloc(i64 4)
  store i32 0, ptr %mallocCall.i.i, align 4
  %mallocCall.i = tail call ptr @malloc(i64 0)
  %mallocCall.i1.i = tail call ptr @malloc(i64 4)
  store i32 5, ptr %mallocCall.i1.i, align 4
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @fmt.1, i32 5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #3

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) }
attributes #3 = { nofree nounwind }
attributes #4 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none) }
attributes #5 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: readwrite, inaccessiblemem: none) }
