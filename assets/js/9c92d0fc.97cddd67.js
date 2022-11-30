"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[434],{3905:(e,t,r)=>{r.r(t),r.d(t,{MDXContext:()=>c,MDXProvider:()=>m,mdx:()=>h,useMDXComponents:()=>u,withMDXComponents:()=>d});var n=r(67294);function a(e,t,r){return t in e?Object.defineProperty(e,t,{value:r,enumerable:!0,configurable:!0,writable:!0}):e[t]=r,e}function o(){return o=Object.assign||function(e){for(var t=1;t<arguments.length;t++){var r=arguments[t];for(var n in r)Object.prototype.hasOwnProperty.call(r,n)&&(e[n]=r[n])}return e},o.apply(this,arguments)}function i(e,t){var r=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),r.push.apply(r,n)}return r}function s(e){for(var t=1;t<arguments.length;t++){var r=null!=arguments[t]?arguments[t]:{};t%2?i(Object(r),!0).forEach((function(t){a(e,t,r[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(r)):i(Object(r)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(r,t))}))}return e}function l(e,t){if(null==e)return{};var r,n,a=function(e,t){if(null==e)return{};var r,n,a={},o=Object.keys(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||(a[r]=e[r]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(n=0;n<o.length;n++)r=o[n],t.indexOf(r)>=0||Object.prototype.propertyIsEnumerable.call(e,r)&&(a[r]=e[r])}return a}var c=n.createContext({}),d=function(e){return function(t){var r=u(t.components);return n.createElement(e,o({},t,{components:r}))}},u=function(e){var t=n.useContext(c),r=t;return e&&(r="function"==typeof e?e(t):s(s({},t),e)),r},m=function(e){var t=u(e.components);return n.createElement(c.Provider,{value:t},e.children)},p={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},f=n.forwardRef((function(e,t){var r=e.components,a=e.mdxType,o=e.originalType,i=e.parentName,c=l(e,["components","mdxType","originalType","parentName"]),d=u(r),m=a,f=d["".concat(i,".").concat(m)]||d[m]||p[m]||o;return r?n.createElement(f,s(s({ref:t},c),{},{components:r})):n.createElement(f,s({ref:t},c))}));function h(e,t){var r=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var o=r.length,i=new Array(o);i[0]=f;var s={};for(var l in t)hasOwnProperty.call(t,l)&&(s[l]=t[l]);s.originalType=e,s.mdxType="string"==typeof e?e:a,i[1]=s;for(var c=2;c<o;c++)i[c]=r[c];return n.createElement.apply(null,i)}return n.createElement.apply(null,r)}f.displayName="MDXCreateElement"},16850:(e,t,r)=>{r.r(t),r.d(t,{assets:()=>c,contentTitle:()=>s,default:()=>f,frontMatter:()=>i,metadata:()=>l,toc:()=>d});var n=r(87462),a=(r(67294),r(3905)),o=r(86341);const i={id:"overview",title:"Overview",sidebar_label:"Overview"},s=void 0,l={unversionedId:"overview",id:"overview",title:"Overview",description:"What is Mariana Trench",source:"@site/documentation/overview.md",sourceDirName:".",slug:"/overview",permalink:"/docs/overview",draft:!1,editUrl:"https://github.com/facebook/mariana-trench/tree/main/documentation/website/documentation/overview.md",tags:[],version:"current",frontMatter:{id:"overview",title:"Overview",sidebar_label:"Overview"},sidebar:"docs",next:{title:"Getting Started",permalink:"/docs/getting-started"}},c={},d=[{value:"What is Mariana Trench",id:"what-is-mariana-trench",level:2},{value:"Background",id:"background",level:2},{value:"Sources and Sinks",id:"sources-and-sinks",level:3},{value:"What does Mariana Trench do?",id:"what-does-mariana-trench-do",level:3},{value:"Usage",id:"usage",level:2},{value:"References",id:"references",level:2}],u=(m="FbOverview",function(e){return console.warn("Component "+m+" was not imported, exported, or provided by MDXProvider as global scope"),(0,a.mdx)("div",e)});var m;const p={toc:d};function f(e){let{components:t,...r}=e;return(0,a.mdx)("wrapper",(0,n.Z)({},p,r,{components:t,mdxType:"MDXLayout"}),(0,a.mdx)("h2",{id:"what-is-mariana-trench"},"What is Mariana Trench"),(0,a.mdx)("p",null,(0,a.mdx)("strong",{parentName:"p"},"Mariana Trench")," is a security focused static analysis platform targeting Android. The tool provides an extensible global taint analysis similar to pre-existing tools like ",(0,a.mdx)("a",{parentName:"p",href:"https://pyre-check.org/docs/pysa-basics"},"Pysa")," for Python. The tool leverages existing static analysis infrastructure (e.g, ",(0,a.mdx)("a",{parentName:"p",href:"https://github.com/facebookincubator/SPARTA"},"SPARTA"),") built on top of ",(0,a.mdx)("a",{parentName:"p",href:"https://github.com/facebook/redex"},"Redex"),"."),(0,a.mdx)("p",null,"By default Mariana Trench analyzes ",(0,a.mdx)("a",{parentName:"p",href:"https://source.android.com/devices/tech/dalvik/dalvik-bytecode"},"dalvik bytecode")," and can work with or without access to the source code."),(0,a.mdx)("h2",{id:"background"},"Background"),(0,a.mdx)("h3",{id:"sources-and-sinks"},"Sources and Sinks"),(0,a.mdx)("p",null,"Under the context of taint analysis ","[1]",', "sources" usually mean sensitive data that originates from users. For example, sources can be users\' passwords or locations. "Sinks" usually mean functions or methods that use data that "flows" from sources, where the term "flow" is generally defined under the context of "information flow" ',"[2]","."),(0,a.mdx)("blockquote",null,(0,a.mdx)("p",{parentName:"blockquote"},"An operation, or series of operations, that uses the value of some object, say x, to derive a value for another, say y, causes a flow from x to y")),(0,a.mdx)("p",null,"As an example, sinks can be a logging API that writes data into a log file."),(0,a.mdx)("h3",{id:"what-does-mariana-trench-do"},"What does Mariana Trench do?"),(0,a.mdx)("p",null,"A flow from sources to sinks indicate that for example user passwords may get logged into a file, which is not desirable and is called as an ",(0,a.mdx)("strong",{parentName:"p"},'"issue"')," under the context of Mariana Trench. Mariana Trench is designed to automatically discover such issues."),(0,a.mdx)("h2",{id:"usage"},"Usage"),(0,a.mdx)("p",null,"The usage of Mariana Trench can be summarized in three steps:"),(0,a.mdx)(o.OssOnly,{mdxType:"OssOnly"},(0,a.mdx)("ol",null,(0,a.mdx)("li",{parentName:"ol"},'Specify customized "sources" and "sinks". (See ',(0,a.mdx)("a",{parentName:"li",href:"/docs/customize-sources-and-sinks"},"Customize Sources and Sinks"),")"),(0,a.mdx)("li",{parentName:"ol"},"Run Mariana Trench on an arbitrary Java repository (with the sources and sinks specified in Step 1), whether it be a repository for an Android application project or for a vanilla (or plain old) Java project."),(0,a.mdx)("li",{parentName:"ol"},"View the analysis results from a web browser. (For steps 2 and 3 see ",(0,a.mdx)("a",{parentName:"li",href:"/docs/getting-started"},"Getting Started"),")"))),(0,a.mdx)(o.FbInternalOnly,{mdxType:"FbInternalOnly"}," ",(0,a.mdx)(u,{mdxType:"FbOverview"})," "),(0,a.mdx)("h2",{id:"references"},"References"),(0,a.mdx)("ol",null,(0,a.mdx)("li",{parentName:"ol"},(0,a.mdx)("a",{parentName:"li",href:"https://dl.acm.org/doi/10.1145/1542476.1542486"},'Tripp, Omer, et al. "TAJ: effective taint analysis of web applications." ACM Sigplan Notices 44.6 (2009): 87-97.')),(0,a.mdx)("li",{parentName:"ol"},(0,a.mdx)("a",{parentName:"li",href:"https://dl.acm.org/doi/10.1145/359636.359712"},'Denning, Dorothy E., and Peter J. Denning. "Certification of programs for secure information flow." Communications of the ACM 20.7 (1977): 504-513.'))))}f.isMDXComponent=!0}}]);