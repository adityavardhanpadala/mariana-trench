"use strict";(self.webpackChunkwebsite=self.webpackChunkwebsite||[]).push([[13],{78665:function(e,t,a){a.d(t,{Z:function(){return E}});var r=a(80102),l=a(67294),n=a(86010),s=a(11022),i=a(39960),c="sidebar_2ahu",m="sidebarItemTitle_2hhb",u="sidebarItemList_2xAf",o="sidebarItem_2UVv",d="sidebarItemLink_1RT6",g="sidebarItemLinkActive_12pM",b=a(95999);function f(e){var t=e.sidebar;return 0===t.items.length?null:l.createElement("nav",{className:(0,n.default)(c,"thin-scrollbar"),"aria-label":(0,b.translate)({id:"theme.blog.sidebar.navAriaLabel",message:"Blog recent posts navigation",description:"The ARIA label for recent posts in the blog sidebar"})},l.createElement("div",{className:(0,n.default)(m,"margin-bottom--md")},t.title),l.createElement("ul",{className:u},t.items.map((function(e){return l.createElement("li",{key:e.permalink,className:o},l.createElement(i.default,{isNavLink:!0,to:e.permalink,className:d,activeClassName:g},e.title))}))))}var v=["sidebar","toc","children"];var E=function(e){var t=e.sidebar,a=e.toc,i=e.children,c=(0,r.Z)(e,v),m=t&&t.items.length>0;return l.createElement(s.Z,c,l.createElement("div",{className:"container margin-vert--lg"},l.createElement("div",{className:"row"},m&&l.createElement("aside",{className:"col col--3"},l.createElement(f,{sidebar:t})),l.createElement("main",{className:(0,n.default)("col",{"col--7":m,"col--9 col--offset-1":!m}),itemScope:!0,itemType:"http://schema.org/Blog"},i),a&&l.createElement("div",{className:"col col--2"},a))))}},40497:function(e,t,a){a.r(t),a.d(t,{default:function(){return u}});var r=a(67294),l=a(78665),n=a(7774),s=a(89521),i="tag_21yA";function c(e){var t=e.letterEntry;return r.createElement("article",null,r.createElement("h2",null,t.letter),r.createElement("ul",{className:"padding--none"},t.tags.map((function(e){return r.createElement("li",{key:e.permalink,className:i},r.createElement(n.Z,e))}))),r.createElement("hr",null))}var m=function(e){var t=e.tags,a=(0,s.listTagsByLetters)(t);return r.createElement("section",{className:"margin-vert--lg"},a.map((function(e){return r.createElement(c,{key:e.letter,letterEntry:e})})))};var u=function(e){var t=e.tags,a=e.sidebar,n=(0,s.translateTagsPageTitle)();return r.createElement(l.Z,{title:n,wrapperClassName:s.ThemeClassNames.wrapper.blogPages,pageClassName:s.ThemeClassNames.page.blogTagsListPage,searchMetadatas:{tag:"blog_tags_list"},sidebar:a},r.createElement("h1",null,n),r.createElement(m,{tags:Object.values(t)}))}},7774:function(e,t,a){a.d(t,{Z:function(){return m}});var r=a(67294),l=a(86010),n=a(39960),s="tag_1Okp",i="tagRegular_3MiF",c="tagWithCount_1HU1";var m=function(e){var t,a=e.permalink,m=e.name,u=e.count;return r.createElement(n.default,{href:a,className:(0,l.default)(s,(t={},t[i]=!u,t[c]=u,t))},m,u&&r.createElement("span",null,u))}}}]);