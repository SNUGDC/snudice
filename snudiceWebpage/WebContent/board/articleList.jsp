<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page trimDirectiveWhitespaces="true" %>
<%@ taglib prefix ="c" uri="http://java.sun.com/jsp/jstl/core" %>
  
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<link rel="icon" href="${root}/favicon.ico" type="image/x-icon"/>
<link rel="shortcut icon" href="${root}/favicon.ico" type="image/x-icon"/>
<link rel="stylesheet" type="text/css" href="${root}/css/commonStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/menuStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/loginModuleStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/boardStyle.css"/>

<script type="text/javascript" src="${root}/javascript/util.js"></script>
<script type="text/javascript" src="${root}/javascript/md5.js"></script>
<script type="text/javascript" src="${root}/javascript/header.js"></script>
<script type="text/javascript" src="${root}/javascript/menuScript.js"></script> 
<script type="text/javascript" src="${root}/javascript/login.js"></script> 
<script type="text/javascript">		
var root = "${root}";
var _userId = "${userId}";
var _role = "${role}";

window.onload = init;

function init()
{
	initHeader();
	initMenu();
	initLogin();
}
</script>

<title>${boardAliasName}</title>
</head>
<body>
	<div class="bodyWrapper">
		<%@ include file="/reuseModule/headerModule.jsp"%>	
		
		<div class="container">	
			<%@ include file="/reuseModule/menuModule.jsp"%>
			
			<div class="leftCenterRightWrapper">			
				<div class="left">
					<%@ include file="/reuseModule/loginModule.jsp"%>
					<div id="mainLeftBottom">
					</div>
				</div>
						
				<div class = "center">
					<div class="contentTitle">${boardAliasName}</div>				
					<div class="frameTop"></div>
					<div class="frameBody">
						<table id = "articleListMain">
							<tr id="articleListHeader">
								<td class="articleListIndex">번호</td>
								<td class="articleListTitle">제목</td>
								<td class="articleListWriter">글쓴이</td>
								<td class="articleListDateTime">날짜</td>
								<td class="articleListReadCount">조회수</td>
							</tr>
							
							<c:forEach var="article" items="${articleList}" varStatus="status">								
								<tr class="article">
									<td class="articleListIndex">${article.articleIndex}</td>
									<td class="articleListTitle">								
										<c:set var="articleReadLink" value="#"/>								
										<c:if test="${canRead == true}">
											<c:set var="articleReadLink" value="${root}/board/articleView.do?boardName=${param.boardName}&amp;articleIndex=${article.articleIndex}&amp;currPage=${param.currPage}"/>
										</c:if>														
															
										<a href="${articleReadLink}">															
											<span>${article.title}</span> 
											<c:if test="${replyCountList[status.index]>0}">
												<span> (${replyCountList[status.index]})</span>
											</c:if>
										</a>
									</td>
									<td class="articleListWriter nickname">																
										<c:if test="${isGMList[status.index]==true}">
											<img class="gmImage" src="${root}/image/board/gm.png"/>
										</c:if>																				
										${article.nickname}
									</td>
									<td class="articleListDateTime">${article.dateTime}</td> 
									<td class="articleListReadCount">${article.readCount}</td>		
								</tr>
							</c:forEach>					 
						</table>
						
						<c:url var="titleKeywordEncoded" value="${titleKeyword}"/>			
						<table id="articleListBottom">
							<tr class="buttonTr">
								<td colspan="3">
									<a href="${root}/board/articleList.do?boardName=${param.boardName}&currPage=0">
										<img src="${root}/image/board/toFirstPage.png"/>																												
									</a>
									<c:if test="${canWrite == true}">
										<a href="${root}/board/articleWriteForm.do?boardName=${param.boardName}&currPage=${param.currPage}">
											<img src="${root}/image/board/articleWrite.png"/>
										</a>
									</c:if>
								</td>								
							</tr>						
							<tr>
								<td class="prevNextPage"> 					
									<c:choose>
										<c:when test="${not empty search}">
											<c:set var="nextUrl" value="${root}/board/articleSearch.do?titleKeyword=${titleKeywordEncoded}&boardName=${param.boardName}&amp;currPage=${param.currPage}&amp;goPrev=true"/>								
										</c:when>
										<c:otherwise>
											<c:set var="nextUrl" value="${root}/board/articleList.do?boardName=${param.boardName}&amp;currPage=${param.currPage}&amp;goPrev=true"/>
										</c:otherwise>
									</c:choose>	
									<a href="${nextUrl}">
										<img src="${root}/image/board/prev.png" alt="앞페이지"/>
									</a> 
								</td>			
									
								<td class="pageNumber">
								<c:forEach var="pageNumber" items="${pageList}">						
									<c:choose>
										<c:when test="${not empty search}">
											<c:set var="nextUrl" value="${root}/board/articleSearch.do?titleKeyword=${titleKeywordEncoded}&boardName=${param.boardName}&amp;currPage=${pageNumber}"/>								
										</c:when>
										<c:otherwise>
											<c:set var="nextUrl" value="${root}/board/articleList.do?boardName=${param.boardName}&amp;currPage=${pageNumber}"/>
										</c:otherwise>
									</c:choose>
												
									<c:choose>
										<c:when test="${pageNumber==param.currPage}">
											<c:set var="formattedPageNumber" value="(${pageNumber})"/>
										</c:when>
										<c:otherwise>
											<c:set var="formattedPageNumber" value="${pageNumber}"/>
										</c:otherwise>
									</c:choose>
									<a href=${nextUrl}>${formattedPageNumber }</a>					
								</c:forEach>
								</td>
								<td class="prevNextPage"> 
									<c:choose>
										<c:when test="${not empty search}">
											<c:set var="nextUrl" value="${root}/board/articleSearch.do?titleKeyword=${titleKeywordEncoded}&boardName=${param.boardName}&amp;currPage=${param.currPage}&amp;goNext=true"/>								
										</c:when>
										<c:otherwise>
											<c:set var="nextUrl" value="${root}/board/articleList.do?boardName=${param.boardName}&amp;currPage=${param.currPage}&amp;goNext=true"/>
										</c:otherwise>
									</c:choose>	
									<a href="${nextUrl}">
										<img src="${root}/image/board/next.png" alt="뒷페이지"/>
									</a> 					
								</td>
							</tr>	
						</table>
					</div>
					<div id="articleSearch" class="frameBottom">
						<form id="articleSearchForm" method="get" action="${root}/board/articleSearch.do">
							<input class="board" name="titleKeyword" type="text"/>
							<input type="submit" value="검색"/>
							<input type="hidden" name="boardName" value="${param.boardName}"/>
							<input type="hidden" name="currPage" value="0"/>							
						</form>
					</div>					
				</div>
									
				<div class="right"></div>
				<div class="dummy"></div>
			</div>
			<div class="containerBottom"></div>
		</div>		
	
		<%@ include file="/reuseModule/footerModule.jsp"%>
	</div>	
</body>
</html>