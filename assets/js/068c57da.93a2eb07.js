"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[683],{3905:(e,n,t)=>{t.r(n),t.d(n,{MDXContext:()=>l,MDXProvider:()=>h,mdx:()=>f,useMDXComponents:()=>p,withMDXComponents:()=>d});var a=t(67294);function i(e,n,t){return n in e?Object.defineProperty(e,n,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[n]=t,e}function m(){return m=Object.assign||function(e){for(var n=1;n<arguments.length;n++){var t=arguments[n];for(var a in t)Object.prototype.hasOwnProperty.call(t,a)&&(e[a]=t[a])}return e},m.apply(this,arguments)}function r(e,n){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var a=Object.getOwnPropertySymbols(e);n&&(a=a.filter((function(n){return Object.getOwnPropertyDescriptor(e,n).enumerable}))),t.push.apply(t,a)}return t}function o(e){for(var n=1;n<arguments.length;n++){var t=null!=arguments[n]?arguments[n]:{};n%2?r(Object(t),!0).forEach((function(n){i(e,n,t[n])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):r(Object(t)).forEach((function(n){Object.defineProperty(e,n,Object.getOwnPropertyDescriptor(t,n))}))}return e}function s(e,n){if(null==e)return{};var t,a,i=function(e,n){if(null==e)return{};var t,a,i={},m=Object.keys(e);for(a=0;a<m.length;a++)t=m[a],n.indexOf(t)>=0||(i[t]=e[t]);return i}(e,n);if(Object.getOwnPropertySymbols){var m=Object.getOwnPropertySymbols(e);for(a=0;a<m.length;a++)t=m[a],n.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(i[t]=e[t])}return i}var l=a.createContext({}),d=function(e){return function(n){var t=p(n.components);return a.createElement(e,m({},n,{components:t}))}},p=function(e){var n=a.useContext(l),t=n;return e&&(t="function"==typeof e?e(n):o(o({},n),e)),t},h=function(e){var n=p(e.components);return a.createElement(l.Provider,{value:n},e.children)},c={inlineCode:"code",wrapper:function(e){var n=e.children;return a.createElement(a.Fragment,{},n)}},g=a.forwardRef((function(e,n){var t=e.components,i=e.mdxType,m=e.originalType,r=e.parentName,l=s(e,["components","mdxType","originalType","parentName"]),d=p(t),h=i,g=d["".concat(r,".").concat(h)]||d[h]||c[h]||m;return t?a.createElement(g,o(o({ref:n},l),{},{components:t})):a.createElement(g,o({ref:n},l))}));function f(e,n){var t=arguments,i=n&&n.mdxType;if("string"==typeof e||i){var m=t.length,r=new Array(m);r[0]=g;var o={};for(var s in n)hasOwnProperty.call(n,s)&&(o[s]=n[s]);o.originalType=e,o.mdxType="string"==typeof e?e:i,r[1]=o;for(var l=2;l<m;l++)r[l]=t[l];return a.createElement.apply(null,r)}return a.createElement.apply(null,t)}g.displayName="MDXCreateElement"},26442:(e,n,t)=>{t.r(n),t.d(n,{assets:()=>s,contentTitle:()=>r,default:()=>c,frontMatter:()=>m,metadata:()=>o,toc:()=>l});var a=t(87462),i=(t(67294),t(3905));const m={id:"shims",title:"Shims",sidebar_label:"Shims"},r=void 0,o={unversionedId:"shims",id:"shims",title:"Shims",description:'What is a "shim"?',source:"@site/documentation/shims.md",sourceDirName:".",slug:"/shims",permalink:"/docs/shims",draft:!1,editUrl:"https://github.com/facebook/mariana-trench/tree/main/documentation/website/documentation/shims.md",tags:[],version:"current",frontMatter:{id:"shims",title:"Shims",sidebar_label:"Shims"},sidebar:"docs",previous:{title:"Models",permalink:"/docs/models"},next:{title:"Configuration",permalink:"/docs/configuration"}},s={},l=[{value:"What is a &quot;shim&quot;?",id:"what-is-a-shim",level:2},{value:"Terminologies",id:"terminologies",level:2},{value:"Specifying Shims",id:"specifying-shims",level:2},{value:"Configuration file",id:"configuration-file",level:3},{value:"Shim Definition",id:"shim-definition",level:3},{value:"Example",id:"example",level:3}],d=(p="FbShims",function(e){return console.warn("Component "+p+" was not imported, exported, or provided by MDXProvider as global scope"),(0,i.mdx)("div",e)});var p;const h={toc:l};function c(e){let{components:n,...t}=e;return(0,i.mdx)("wrapper",(0,a.Z)({},h,t,{components:n,mdxType:"MDXLayout"}),(0,i.mdx)("h2",{id:"what-is-a-shim"},'What is a "shim"?'),(0,i.mdx)("p",null,'We can think of a \u201cshim\u201d as the ability to say: "a call to a method matching\n',(0,i.mdx)("inlineCode",{parentName:"p"},"<shimmed-method>")," also implies calls to methods matching ",(0,i.mdx)("inlineCode",{parentName:"p"},"<shim-target-1>"),",\n",(0,i.mdx)("inlineCode",{parentName:"p"},"<shim-target-2>"),", \u2026, ",(0,i.mdx)("inlineCode",{parentName:"p"},"<shim-target-n>"),".  This allows us to manually augment the\ncall-graph with artificial calls to methods at specific call-sites. This is\nuseful for simulating events in the android application by mimicking calls to\nthe known event-handlers which allows us to capture dataflows otherwise missed\ndue to the missing invocation of event-handlers."),(0,i.mdx)("p",null,"For example, when a new fragment is added to a ",(0,i.mdx)("inlineCode",{parentName:"p"},"FragmentManager"),", the\n",(0,i.mdx)("inlineCode",{parentName:"p"},"FragmentManager")," is responsible for moving them through its lifecycle states\n(",(0,i.mdx)("a",{parentName:"p",href:"https://developer.android.com/guide/fragments/lifecycle#fragmentmanager"},"reference"),").\nBut we do not see the calls to the lifecycle event handlers on the new instance\nof the fragment and can miss flows. We can define shims as follows to fill in\nsuch missing links."),(0,i.mdx)("pre",null,(0,i.mdx)("code",{parentName:"pre",className:"language-java"},"class MainActivity extends AppCompatActivity {\n    @Override\n    protected void onCreate(Bundle savedinstanceState) {\n        if (savedInstanceState == null) {\n            Fragment myFragment = new MyFragment();\n            // highlight-next-line\n            myFragment.setArguments(getTaintedBundle());  // Source\n            getSupportFragmentManager().beginTransaction()\n                .setReorderingAllowed(true)\n                // highlight-next-line\n                .add(R.id.fragment_container_view, myFragment, null)  // FragmentManager handles `myFragment`'s lifecycle.\n                .commit();\n        }\n    }\n}\n\nclass MyFragment extends Fragment {\n    @Override\n    public void onCreate(Bundle savedInstanceState) {\n        // highlight-next-line\n        sink(getArguments()); // Issue if `mArguments` is tainted using `setArguments()`\n    }\n}\n")),(0,i.mdx)("p",null,"In this case, we can define a shim on ",(0,i.mdx)("inlineCode",{parentName:"p"},"FragmentTrasaction"),"'s ",(0,i.mdx)("inlineCode",{parentName:"p"},"add()"),"  to\n",(0,i.mdx)("em",{parentName:"p"},"trigger")," the lifecycle wrapper method of it's ",(0,i.mdx)("inlineCode",{parentName:"p"},"Fragment")," argument to mimic the\nandroid's lifecycle management to catch this flow."),(0,i.mdx)("pre",null,(0,i.mdx)("code",{parentName:"pre",className:"language-json"},'{\n    "find": "methods",\n    "where": [\n        {\n            "constraint": "signature",\n            "pattern": "Landroidx/fragment/app/FragmentTransaction;\\\\.add:\\\\(ILandroidx/fragment/app/Fragment;.*"\n        }\n    ],\n    "shim": {\n        "callees": [\n            {\n                "type_of": "Argument(2)",\n                "lifecycle_name": "xfragment_lifecycle_wrapper"\n            }\n        ]\n    }\n},\n')),(0,i.mdx)("h2",{id:"terminologies"},"Terminologies"),(0,i.mdx)("ul",null,(0,i.mdx)("li",{parentName:"ul"},"shimmed-method: The method matching the ",(0,i.mdx)("inlineCode",{parentName:"li"},'"where"')," clause in the shim generator."),(0,i.mdx)("li",{parentName:"ul"},"shim-target: Method matching each of the ",(0,i.mdx)("inlineCode",{parentName:"li"},'"callees"')," object specified in the shim generator."),(0,i.mdx)("li",{parentName:"ul"},"parameters map: Mapping for arguments from shim-target to shimmed-method.")),(0,i.mdx)("p",null,"All callsites of the ",(0,i.mdx)("inlineCode",{parentName:"p"},"shimmed-method")," implies calls to all the ",(0,i.mdx)("inlineCode",{parentName:"p"},"shim-targets"),"\nspecified with arguments propagated from the shimmed-method to shim-target based\non the parameters map."),(0,i.mdx)("h2",{id:"specifying-shims"},"Specifying Shims"),(0,i.mdx)("h3",{id:"configuration-file"},"Configuration file"),(0,i.mdx)("p",null,"The Mariana Trench binary consumes shim configuration files specified with the\nargument ",(0,i.mdx)("inlineCode",{parentName:"p"},"--shims-paths"),". Each file contains a json array consisting of shim\ndefinitions. Default set of shims can found in\n",(0,i.mdx)("a",{parentName:"p",href:"https://github.com/facebook/mariana-trench/blob/main/configuration/shims.json"},"shims.json")),(0,i.mdx)("h3",{id:"shim-definition"},"Shim Definition"),(0,i.mdx)("p",null,"Each shim definition object consists of the following keys:"),(0,i.mdx)("ul",null,(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"find"),": Currently only option is ",(0,i.mdx)("inlineCode",{parentName:"p"},"methods"),".")),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"where"),': A list of "constraints" which identifies the "shimmed-method". This is same as in ',(0,i.mdx)("a",{parentName:"p",href:"/docs/models#generators"},"model generators"),".")),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"shim"),': A list of "callees" each of which identifies a "shim-target". Each ',(0,i.mdx)("inlineCode",{parentName:"p"},"callees")," object needs to define the following:"),(0,i.mdx)("h4",{parentName:"li",id:"receiver-for-the-shim-target"},"Receiver for the shim-target"),(0,i.mdx)("p",{parentName:"li"},"Receiver can be defined using one of the following keys:"),(0,i.mdx)("ul",{parentName:"li"},(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"static"),": Used to specify static methods as shim-targets."),(0,i.mdx)("p",{parentName:"li"},"Expected value: A string specifying the dex class containing the shim-target method.")),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"type_of"),": Used to specify an instance argument of the shimmed-method as the receiver."),(0,i.mdx)("p",{parentName:"li"},"Expected value: A string specifying the ",(0,i.mdx)("em",{parentName:"p"},"port"),"/",(0,i.mdx)("em",{parentName:"p"},"access path")," of the shimmed-method.")),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"reflected_type_of"),": Used to specify a reflection argument of the shimmed-method as the receiver."),(0,i.mdx)("p",{parentName:"li"},"Expected value: A string specifying the ",(0,i.mdx)("em",{parentName:"p"},"port/access path")," of the shimmed-method. The type of the specified shimmed-method argument ",(0,i.mdx)("em",{parentName:"p"},"must")," be: ",(0,i.mdx)("inlineCode",{parentName:"p"},"Ljava/lang/Class;"),"."))),(0,i.mdx)("h4",{parentName:"li",id:"method-to-call"},"Method to call"),(0,i.mdx)("p",{parentName:"li"},"Method can be defined using one of the following keys:"),(0,i.mdx)("ul",{parentName:"li"},(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"method_name"),": Used to specify an existing method of the receiver as the shim-target."),(0,i.mdx)("p",{parentName:"li"},"Expected value: A string specifying the dex proto of the method. This is of the form: ",(0,i.mdx)("inlineCode",{parentName:"p"},"<method name>:(<parameter types>)<return type>"),".")),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("p",{parentName:"li"},(0,i.mdx)("inlineCode",{parentName:"p"},"lifecycle_name"),": Used to specify lifecycle method specified using the option ",(0,i.mdx)("inlineCode",{parentName:"p"},"--lifecycles-paths")," as the shim-target."),(0,i.mdx)("p",{parentName:"li"},"Expected value: A string matching the ",(0,i.mdx)("inlineCode",{parentName:"p"},"method_name")," specified in the lifecycle configuration."))),(0,i.mdx)("h4",{parentName:"li",id:"parameters-map-optional"},"Parameters map (optional)"),(0,i.mdx)("p",{parentName:"li"},"A map specifying how the parameters of the shimmed-method should be propagated to the shim-target.\nIf not specified, each argument of the shim-target is mapped to the first\nargument of the shimmed-method with the matching type."),(0,i.mdx)("p",{parentName:"li"},'Expected format is as follows where the "key" refers to the shim-target and the value refers to the shimmed-method.'),(0,i.mdx)("pre",{parentName:"li"},(0,i.mdx)("code",{parentName:"pre",className:"language-json"},'"parameters_map": {\n    "Argument(<int>)": "Argument(<int>)",\n    ...\n}\n')))),(0,i.mdx)("h3",{id:"example"},"Example"),(0,i.mdx)("pre",null,(0,i.mdx)("code",{parentName:"pre",className:"language-java",metastring:"showLineNumbers",showLineNumbers:!0},"class TargetA {\n    void methodA(Object o) {}\n}\n\nclass TargetB {\n    void methodB(Object o1, Object o2) {}\n}\n\nclass TargetC {\n    static void methodC(Object o) {}\n}\n\nclass Shimmed {\n    void shimMe(A a, Class b, Object o) {}\n}\n\nclass Test {\n    void test() {\n        new Shimmed().shimMe(new TargetA(), TargetB.class, new Object());\n    }\n}\n")),(0,i.mdx)("p",null,"Shim definitions:"),(0,i.mdx)("pre",null,(0,i.mdx)("code",{parentName:"pre",className:"language-json"},'{\n    "find": "methods",\n    "where": [\n        {\n            "constraint": "signature",\n            "pattern": "LShimmed;\\\\.shimMe:\\\\(LTargetA;Ljava/lang/Class;Ljava/lang/Object;\\\\)V"\n        }\n    ],\n    "shim": {\n        "callees": [\n            {\n                "type_of": "Argument(1)",\n                "method_name": "methodA:(Ljava/lang/Object;)V"\n            },\n            {\n                "reflected_type_of": "Argument(2)",\n                "method_name": "methodB:(Ljava/lang/Object;Ljava/lang/Object;)V",\n                "parameters_map": {\n                    "Argument(2)": "Argument(3)"\n                }\n            },\n            {\n                "static": "LTargetC;",\n                "method_name": "method:C(Ljava/lang/Object;)V",\n                "parameters_map": {\n                    "Argument(0)": "Argument(3)"\n                }\n            }\n        ]\n    }\n},\n')),(0,i.mdx)("p",null,"With this shim definition, the call to method ",(0,i.mdx)("inlineCode",{parentName:"p"},"shimMe()")," on line X will introduce calls to:"),(0,i.mdx)("ul",null,(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("inlineCode",{parentName:"li"},"LTargetA.methodA:(Ljava/lang/Object;)V")," where argument ",(0,i.mdx)("inlineCode",{parentName:"li"},"o")," is inferred to be ",(0,i.mdx)("inlineCode",{parentName:"li"},"Argument(3)")," of method ",(0,i.mdx)("inlineCode",{parentName:"li"},"LShimmed;.shimMe:(LTargetA;Ljava/lang/Class;Ljava/lang/Object;)V"),"."),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("inlineCode",{parentName:"li"},"LTargetB.methodB:(Ljava/lang/Object;)V")," where argument ",(0,i.mdx)("inlineCode",{parentName:"li"},"o2")," is mapped to ",(0,i.mdx)("inlineCode",{parentName:"li"},"Argument(3)")," of method ",(0,i.mdx)("inlineCode",{parentName:"li"},"LShimmed;.shimMe:(LTargetA;Ljava/lang/Class;Ljava/lang/Object;)V")," as specified. Note that argument ",(0,i.mdx)("inlineCode",{parentName:"li"},"Class b")," of the shimmed-method is resolved to be ",(0,i.mdx)("inlineCode",{parentName:"li"},"TargetB")," at the callsite."),(0,i.mdx)("li",{parentName:"ul"},(0,i.mdx)("inlineCode",{parentName:"li"},"LTargetC.methodC:()V")," where argument ",(0,i.mdx)("inlineCode",{parentName:"li"},"o")," is mapped to ",(0,i.mdx)("inlineCode",{parentName:"li"},"Argument(3)")," of method ",(0,i.mdx)("inlineCode",{parentName:"li"},"LShimmed;.shimMe:(LTargetA;Ljava/lang/Class;Ljava/lang/Object;)V")," as specified. Note here that we specify ",(0,i.mdx)("inlineCode",{parentName:"li"},"Argument(0)")," of the shim-target in the parameters_map as this is a static method.")),(0,i.mdx)("p",null,"Note that when issues are found due to taint flow through shimmed-method to\nshim-target, the trace following the call-site of the ",(0,i.mdx)("em",{parentName:"p"},"shimmed-method")," will be\nthe ",(0,i.mdx)("em",{parentName:"p"},"shim-target")," and a feature ",(0,i.mdx)("inlineCode",{parentName:"p"},"via-shim:<shimmed-method>")," will be introduced\nat that point."),(0,i.mdx)("p",null,"Sample shim definitions ",(0,i.mdx)("a",{parentName:"p",href:"https://github.com/facebook/mariana-trench/blob/main/source/tests/integration/end-to-end/code/shims/shims.json"},"here"),"."),(0,i.mdx)(d,{mdxType:"FbShims"}))}c.isMDXComponent=!0}}]);