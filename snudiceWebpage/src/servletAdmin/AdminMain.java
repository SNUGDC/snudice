package servletAdmin;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import constant.Const;

/**
 * Servlet implementation class AdminMain
 */
@WebServlet("/AdminMain")
public class AdminMain extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public AdminMain() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {		
		HttpSession session = request.getSession();
		session.setMaxInactiveInterval(3600);
		session.setAttribute("root",Const.root);
		session.setAttribute("userId",request.getUserPrincipal().getName());
		
		System.out.println(request.getAttribute("test"));
		
		
		String nextPage = "admin/adminMain.jsp";	
		RequestDispatcher view = request.getRequestDispatcher(nextPage);
		view.forward(request, response);	
	}
}
