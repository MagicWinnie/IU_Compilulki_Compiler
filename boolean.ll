; ModuleID = 'Boolean'
source_filename = "boolean"

%Boolean = type { i1 }

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1

define void @Boolean_Create_Default(ptr %0, i1 %1) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 %1, ptr %boolFieldPtr, align 1
  ret void
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

