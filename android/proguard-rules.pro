# ============================================================
# ProGuard/R8 rules for Qt Android application
# ============================================================

# --- Qt framework classes (required for Qt to function) ---
-keep class org.qtproject.** { *; }
-keep interface org.qtproject.** { *; }

# --- Keep native method declarations ---
-keepclasseswithmembernames class * {
    native <methods>;
}

# --- AndroidX core (used in dependencies) ---
-keep class androidx.core.** { *; }

# --- Keep application entry points ---
-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider

# --- Keep classes referenced from AndroidManifest.xml ---
-keep class org.qtproject.qt.android.bindings.QtActivity { *; }
-keep class org.qtproject.qt.android.bindings.QtApplication { *; }

# --- Preserve annotations ---
-keepattributes *Annotation*
-keepattributes Signature
-keepattributes InnerClasses
-keepattributes EnclosingMethod

# --- Suppress warnings for Qt internal classes ---
-dontwarn org.qtproject.**
