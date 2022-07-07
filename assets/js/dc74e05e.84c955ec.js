"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[48],{3905:function(e,t,n){n.r(t),n.d(t,{MDXContext:function(){return c},MDXProvider:function(){return m},mdx:function(){return f},useMDXComponents:function(){return u},withMDXComponents:function(){return d}});var o=n(67294);function r(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function a(){return a=Object.assign||function(e){for(var t=1;t<arguments.length;t++){var n=arguments[t];for(var o in n)Object.prototype.hasOwnProperty.call(n,o)&&(e[o]=n[o])}return e},a.apply(this,arguments)}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);t&&(o=o.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,o)}return n}function s(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){r(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function l(e,t){if(null==e)return{};var n,o,r=function(e,t){if(null==e)return{};var n,o,r={},a=Object.keys(e);for(o=0;o<a.length;o++)n=a[o],t.indexOf(n)>=0||(r[n]=e[n]);return r}(e,t);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);for(o=0;o<a.length;o++)n=a[o],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(r[n]=e[n])}return r}var c=o.createContext({}),d=function(e){return function(t){var n=u(t.components);return o.createElement(e,a({},t,{components:n}))}},u=function(e){var t=o.useContext(c),n=t;return e&&(n="function"==typeof e?e(t):s(s({},t),e)),n},m=function(e){var t=u(e.components);return o.createElement(c.Provider,{value:t},e.children)},p={inlineCode:"code",wrapper:function(e){var t=e.children;return o.createElement(o.Fragment,{},t)}},h=o.forwardRef((function(e,t){var n=e.components,r=e.mdxType,a=e.originalType,i=e.parentName,c=l(e,["components","mdxType","originalType","parentName"]),d=u(n),m=r,h=d["".concat(i,".").concat(m)]||d[m]||p[m]||a;return n?o.createElement(h,s(s({ref:t},c),{},{components:n})):o.createElement(h,s({ref:t},c))}));function f(e,t){var n=arguments,r=t&&t.mdxType;if("string"==typeof e||r){var a=n.length,i=new Array(a);i[0]=h;var s={};for(var l in t)hasOwnProperty.call(t,l)&&(s[l]=t[l]);s.originalType=e,s.mdxType="string"==typeof e?e:r,i[1]=s;for(var c=2;c<a;c++)i[c]=n[c];return o.createElement.apply(null,i)}return o.createElement.apply(null,n)}h.displayName="MDXCreateElement"},32639:function(e,t,n){n.r(t),n.d(t,{assets:function(){return d},contentTitle:function(){return l},default:function(){return p},frontMatter:function(){return s},metadata:function(){return c},toc:function(){return u}});var o=n(83117),r=n(80102),a=(n(67294),n(3905)),i=["components"],s={id:"configuration",title:"Configuration",sidebar_label:"Configuration"},l=void 0,c={unversionedId:"configuration",id:"configuration",title:"Configuration",description:"Mariana Trench is highly configurable and we recommend that you invest time into adjusting the tool to your specific use cases. At Facebook, we have dedicated security engineers that will spend a significant amount of their time adding new rules and model generators to improve the analysis results.",source:"@site/documentation/configuration.md",sourceDirName:".",slug:"/configuration",permalink:"/docs/configuration",draft:!1,editUrl:"https://github.com/facebook/mariana-trench/tree/main/documentation/website/documentation/configuration.md",tags:[],version:"current",frontMatter:{id:"configuration",title:"Configuration",sidebar_label:"Configuration"},sidebar:"docs",previous:{title:"Models",permalink:"/docs/models"},next:{title:"Contribution",permalink:"/docs/contribution"}},d={},u=[{value:"Command Line Options",id:"command-line-options",level:2},{value:"<code>--apk-path</code>",id:"--apk-path",level:4},{value:"<code>--output-directory OUTPUT_DIRECTORY</code>",id:"--output-directory-output_directory",level:4},{value:"<code>--system-jar-configuration-path SYSTEM_JAR_CONFIGURATION_PATH</code>",id:"--system-jar-configuration-path-system_jar_configuration_path",level:4},{value:"<code>--rules-paths RULES_PATHS</code>",id:"--rules-paths-rules_paths",level:4},{value:"<code>--source-root-directory SOURCE_ROOT_DIRECTORY</code>",id:"--source-root-directory-source_root_directory",level:4},{value:"<code>--model-generator-configuration-paths MODEL_GENERATOR_CONFIGURATION_PATHS</code>",id:"--model-generator-configuration-paths-model_generator_configuration_paths",level:4},{value:"<code>--model-generator-search-paths MODEL_GENERATOR_SEARCH_PATHS</code>",id:"--model-generator-search-paths-model_generator_search_paths",level:4},{value:"<code>--maximum-source-sink-distance MAXIMUM_SOURCE_SINK_DISTANCE</code>",id:"--maximum-source-sink-distance-maximum_source_sink_distance",level:4}],m={toc:u};function p(e){var t=e.components,n=(0,r.Z)(e,i);return(0,a.mdx)("wrapper",(0,o.Z)({},m,n,{components:t,mdxType:"MDXLayout"}),(0,a.mdx)("p",null,"Mariana Trench is highly configurable and we recommend that you invest time into adjusting the tool to your specific use cases. At Facebook, we have dedicated security engineers that will spend a significant amount of their time adding new rules and model generators to improve the analysis results."),(0,a.mdx)("p",null,"This page will cover the more important, non-trivial configuration options. Note that you will spend most of your time configuring Mariana Trench writing model generators. These are covered in the ",(0,a.mdx)("a",{parentName:"p",href:"/docs/models"},"next section"),"."),(0,a.mdx)("h2",{id:"command-line-options"},"Command Line Options"),(0,a.mdx)("p",null,"You can get a full set of options by running ",(0,a.mdx)("inlineCode",{parentName:"p"},"mariana-trench --help"),". The following is an abbreviated version of the output."),(0,a.mdx)("pre",null,(0,a.mdx)("code",{parentName:"pre",className:"language-shell"},"$ mariana-trench --help\n\nTarget arguments:\n  --apk-path APK_PATH   The APK to analyze.\n\nOutput arguments:\n  --output-directory OUTPUT_DIRECTORY\n                        The directory to store results in.\n\nConfiguration arguments:\n  --system-jar-configuration-path SYSTEM_JAR_CONFIGURATION_PATH\n                        A JSON configuration file with a list of paths to the system jars.\n  --rules-paths RULES_PATHS\n                        A `;`-separated list of rules files and directories containing rules files.\n  --repository-root-directory REPOSITORY_ROOT_DIRECTORY\n                        The root of the repository. Resulting paths will be relative to this.\n  --source-root-directory SOURCE_ROOT_DIRECTORY\n                        The root where source files for the APK can be found.\n  --model-generator-configuration-paths MODEL_GENERATOR_CONFIGURATION_PATHS\n                        A `;`-separated list of paths specifying JSON configuration files. Each file is a list of paths to JSON model generators relative to the\n                        configuration file or names of CPP model generators.\n  --model-generator-search-paths MODEL_GENERATOR_SEARCH_PATHS\n                        A `;`-separated list of paths where we look up JSON model generators.\n  --maximum-source-sink-distance MAXIMUM_SOURCE_SINK_DISTANCE\n                        Limits the distance of sources and sinks from a trace entry point.\n")),(0,a.mdx)("h4",{id:"--apk-path"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--apk-path")),(0,a.mdx)("p",null,"Mariana Trench analyzes Dalvik bytecode. You provide it with the android app (APK) to analyze."),(0,a.mdx)("h4",{id:"--output-directory-output_directory"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--output-directory OUTPUT_DIRECTORY")),(0,a.mdx)("p",null,"The output of the analysis is a file containing metadata about the particular run in JSON format as well as sharded files containing data flow specifications for every method in the APK. These files need to be processed by SAPP (see ",(0,a.mdx)("a",{parentName:"p",href:"/docs/getting-started"},"Getting Started"),") after the analysis. The flag specifies where these files are saved."),(0,a.mdx)("h4",{id:"--system-jar-configuration-path-system_jar_configuration_path"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--system-jar-configuration-path SYSTEM_JAR_CONFIGURATION_PATH")),(0,a.mdx)("p",null,"This path points to a json file containing a list of ",(0,a.mdx)("inlineCode",{parentName:"p"},".jar")," files that the analysis should include in the analysis. It's important that this contains at least the ",(0,a.mdx)("inlineCode",{parentName:"p"},"android.jar")," on your system. This file is typically located in your android SDK distribution at ",(0,a.mdx)("inlineCode",{parentName:"p"},"$ANDROID_SDK/platforms/android-30/android.jar"),". Without the ",(0,a.mdx)("inlineCode",{parentName:"p"},"android.jar"),", Mariana Trench will not know about many methods from the standard library that might be important for your model generators."),(0,a.mdx)("h4",{id:"--rules-paths-rules_paths"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--rules-paths RULES_PATHS")),(0,a.mdx)("p",null,"A ",(0,a.mdx)("inlineCode",{parentName:"p"},";")," separated search path pointing to files and directories containing rules files. These files specify what taint flows Mariana Trench should look for. Check out the ",(0,a.mdx)("a",{parentName:"p",href:"https://github.com/facebook/mariana-trench/blob/main/configuration/rules.json#L2-L13"},(0,a.mdx)("inlineCode",{parentName:"a"},"rules.json"))," that's provided by default. It specifies that we want to find flows from user controlled input (",(0,a.mdx)("inlineCode",{parentName:"p"},"ActivityUserInput"),") into ",(0,a.mdx)("inlineCode",{parentName:"p"},"CodeExecution")," sinks and that this constitutes a remote code execution."),(0,a.mdx)("h4",{id:"--source-root-directory-source_root_directory"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--source-root-directory SOURCE_ROOT_DIRECTORY")),(0,a.mdx)("p",null,"Mariana Trench will do a source indexing path before the analysis. This is because Dalvik/Java bytecode does not contain complete location information, only filenames (not paths) and line numbers. The index is later used to emit precise locations."),(0,a.mdx)("h4",{id:"--model-generator-configuration-paths-model_generator_configuration_paths"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--model-generator-configuration-paths MODEL_GENERATOR_CONFIGURATION_PATHS")),(0,a.mdx)("p",null,"A ",(0,a.mdx)("inlineCode",{parentName:"p"},";")," separated set of files containing the names of model generators to run. See ",(0,a.mdx)("a",{parentName:"p",href:"https://github.com/facebook/mariana-trench/blob/main/configuration/default_generator_config.json"},(0,a.mdx)("inlineCode",{parentName:"a"},"default_generator_config.json"))," for an example."),(0,a.mdx)("h4",{id:"--model-generator-search-paths-model_generator_search_paths"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--model-generator-search-paths MODEL_GENERATOR_SEARCH_PATHS")),(0,a.mdx)("p",null,"A ",(0,a.mdx)("inlineCode",{parentName:"p"},";")," separated search path where Mariana Trench will try to find the model generators specified in the generator configuration."),(0,a.mdx)("h4",{id:"--maximum-source-sink-distance-maximum_source_sink_distance"},(0,a.mdx)("inlineCode",{parentName:"h4"},"--maximum-source-sink-distance MAXIMUM_SOURCE_SINK_DISTANCE")),(0,a.mdx)("p",null,"For performance reasons it can be useful to limit the maximum length of a trace Mariana Trench tries to find (note that longer traces also tend to be harder to interpret). Due to the modular nature of the analysis the value specified here limits the maximum length from the trace root to the source, and from the trace root to the sink. This means found traces can have length of ",(0,a.mdx)("inlineCode",{parentName:"p"},"2 x MAXIMUM_SOURCE_SINK_DISTANCE"),"."))}p.isMDXComponent=!0}}]);