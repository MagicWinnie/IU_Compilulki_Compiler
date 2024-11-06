; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Main = type {}

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @Boolean_Constructor(ptr %0, i1 %1) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 %1, ptr %boolFieldPtr, align 1
  ret void
}

define void @Boolean_print(ptr %0) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  %loadBool = load i1, ptr %boolFieldPtr, align 1
  %extBool = zext i1 %loadBool to i32
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, i32 %extBool)
  ret void
}

declare i32 @printf(ptr, ...)

define void @Main_Create_Default(ptr %0) {
entry:
  %eq = alloca i32, align 4
  store i1 false, ptr %eq, align 1
  call void @Boolean_print(ptr %eq)
  ret void
}

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Create_Default(ptr %main_object)
  ret i32 0
}
