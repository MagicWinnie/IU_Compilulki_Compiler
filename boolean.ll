; ModuleID = 'Boolean'
source_filename = "boolean"

%Boolean = type { i1 }

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %extBool = zext i1 %loadBool to i32
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, i32 %extBool)
  ret void
}

declare i32 @printf(ptr, ...)

