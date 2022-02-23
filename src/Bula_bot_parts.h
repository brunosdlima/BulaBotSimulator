#pragma once

void DoWork() {
    std::thread worker(DoWork);
    worker.join();
}

class Timer {
private:
    std::chrono::time_point <std::chrono::high_resolution_clock> Start_time;
    std::chrono::time_point <std::chrono::high_resolution_clock> End_time;
public:
    Timer() {
        Start_time = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        Stop();
    }
    void Stop() {
        End_time = std::chrono::high_resolution_clock::now();
        auto Start = std::chrono::time_point_cast<std::chrono::microseconds> (Start_time).time_since_epoch().count();
        auto End = std::chrono::time_point_cast<std::chrono::microseconds> (End_time).time_since_epoch().count();
        auto Class_run_time = End - Start;
        Class_run_time = Class_run_time * (float)0.001;
        std::cout << "The time spent since timer call and next bracket is : " << Class_run_time << " ms" << std::endl;
    }
};
class Point {
private:
    float x, y;
public:
    Point() {
        x = 0.0;
        y = 0.0;
    }
    Point(const float& a, const float& b) : x(a), y(b) {
    }
    ~Point() {
#ifdef _DEBUG 
        std::cout << "Point was destructed " << std::endl;
#endif
    }

    void set_point(const float& x_coord, const float& y_coord) {
        (*this).x = x_coord;
        (*this).y = y_coord;
    }
    float const get_point_x() const {
        return (*this).x;
    }
    float const get_point_y() const {
        return (*this).y;
    }
    void print_coordinates() const {
        std::cout << "x coordinate:  " << x << " y coordinate:  " << y << std::endl;
    }
};
class Four_bar_linkage {
private:
    float angle12;
    float angle23;
    Point O1;
    Point O2;
    Point A;
    Point B;
public:
    Four_bar_linkage(const float& angle1, const float& angle2, const Point& p1, const Point& p2, const Point& p3, const Point& p4) : angle12(angle1), angle23(angle2), O1(p1), O2(p2), A(p3), B(p4) {
    };
    Four_bar_linkage() {
        angle12 = 0.0;
        angle23 = 0.0;
    };
    ~Four_bar_linkage() {
#ifdef _DEBUG 
        std::cout << "Four bar mechanism was destructed" << std::endl;
#endif   
    };

    void four_bar_set_point(const char& current_vertex, const float& x_coord, const float& y_coord) {
        if (current_vertex == 'a') {
            O1.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'b') {
            A.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'c') {
            B.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'd') {
            O2.set_point(x_coord, y_coord);
        }
        else {
            std::cout << "There is no such vertex for current four bar linkage" << std::endl;
        }
    }
    void set_four_bar_points(const float& phi, const float& theta_1, const Point& O_1, const float& a, const float& b, const float& c, const float& d) {
        float XA_rot, YA_rot, f, beta, gamma, theta, O2_rot_x, O2_rot_y, B_rot_x, B_rot_y;

        O1 = O_1;
        O2.set_point(O1.get_point_x() + a * cos(phi), O1.get_point_y() + a * sin(phi));
        A.set_point(O1.get_point_x() + b * cos(theta_1), O1.get_point_y() + b * sin(theta_1));
        XA_rot = (A.get_point_x() - O1.get_point_x()) * cos(phi) + (A.get_point_y() - O1.get_point_y()) * sin(phi);
        YA_rot = -(A.get_point_x() - O1.get_point_x()) * sin(phi) + (A.get_point_y() - O1.get_point_y()) * cos(phi);
        f = sqrt(((a - XA_rot) * (a - XA_rot)) + ((YA_rot) * (YA_rot)));
        beta = acos(((b * b) - (c * c) + (f * f)) / (2 * b * f));
        gamma = atan2((YA_rot), (a - XA_rot));
        theta = ((float)(180 * 3.14159265 / 180.0)) - gamma - beta;

        O2_rot_x = ((O2.get_point_x() - O1.get_point_x()) * cos(phi)) + ((O2.get_point_y() - O1.get_point_y()) * sin(phi));
        O2_rot_y = -((O2.get_point_x() - O1.get_point_x()) * sin(phi)) + ((O2.get_point_y() - O1.get_point_y()) * cos(phi));

        B_rot_x = O2_rot_x + d * cos(theta);
        B_rot_y = O2_rot_y + d * sin(theta);

        B.set_point(O1.get_point_x() + ((B_rot_x)*cos(-phi)) + ((B_rot_y)*sin(-phi)), O1.get_point_y() - ((B_rot_x)*sin(-phi)) + ((B_rot_y)*cos(-phi)));

    }
    const Point* four_bar_get_point(const char& current_vertex) const {
        if (current_vertex == 'a') {
            return &O1;
        }
        else if (current_vertex == 'b') {
            return &A;
        }
        else if (current_vertex == 'c') {
            return &B;
        }
        else if (current_vertex == 'd') {
            return &O2;
        }
        else {
            std::cout << "There is no such vertex for current four bar linkage" << std::endl;
            return 0;
        }
    }
    void print_four_bar_coordinates() const {
        std::cout << "Angle 1 " << " x coordinate:  " << angle12 * (float)(180 / 3.14159265) << "degrees" << std::endl;
        std::cout << "Angle 2 " << " x coordinate:  " << angle23 * (float)(180 / 3.14159265) << "degrees" << std::endl;
        std::cout << "Coordinates of point a in the four bar linkage mechanism: " << " x coordinate:  " << O1.get_point_x() << " y coordinate:  " << O1.get_point_y() << std::endl;
        std::cout << "Coordinates of point b in the four bar linkage mechanism: " << " x coordinate:  " << O2.get_point_x() << " y coordinate:  " << O2.get_point_y() << std::endl;
        std::cout << "Coordinates of point c in the four bar linkage mechanism: " << " x coordinate:  " << A.get_point_x() << " y coordinate:  " << A.get_point_y() << std::endl;
        std::cout << "Coordinates of point d in the four bar linkage mechanism: " << " x coordinate:  " << B.get_point_x() << " y coordinate:  " << B.get_point_y() << std::endl;
    }
};
class Triangle {
private:
    float angle12;
    Point A;
    Point B;
    Point C;
public:
    Triangle() {
        angle12 = 0;
    }
    Triangle(const float& angle, const Point& p1, const Point& p2, const Point& p3) : angle12(angle), A(p1), B(p2), C(p3) {
    }
    ~Triangle() {
#ifdef _DEBUG 
        std::cout << "Triangle was destructed" << std::endl;
#endif 
    }

    const float const get_triangle_angle() const {
        return angle12;
    }
    void triangle_set_point(const char& current_vertex, const float& x_coord, const float& y_coord) {
        if (current_vertex == 'a') {
            A.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'b') {
            B.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'c') {
            C.set_point(x_coord, y_coord);
        }
        else {
            std::cout << "There is no such vertex for current triangle" << std::endl;
        }
    }
    void print_triangle_coordinates() const {
        std::cout << "Coordinates of point a in the triangle: " << " x coordinate:  " << A.get_point_x() << " y coordinate:  " << A.get_point_y() << std::endl;
        std::cout << "Coordinates of point b in the triangle: " << " x coordinate:  " << B.get_point_x() << " y coordinate:  " << B.get_point_y() << std::endl;
        std::cout << "Coordinates of point c in the triangle: " << " x coordinate:  " << C.get_point_x() << " y coordinate:  " << C.get_point_y() << std::endl;
        std::cout << "Angle of the triangle: " << angle12 << std::endl;
    } // Make it virtual and change grip as well
    void set_triangle_points(const float& phi, const Point& O_1, const Point& O_2, const float& a) {
        //phi is the angle between point C and the ground, a is the link from point A to point C
        angle12 = phi;
        A = O_1;
        B = O_2;
        C.set_point(A.get_point_x() + (a * cos(phi)), A.get_point_y() + (a * sin(phi)));
    }
    const Point* triangle_get_point(const char& current_vertex) const {
        if (current_vertex == 'a') {
            return &A;
        }
        else if (current_vertex == 'b') {
            return &B;
        }
        else if (current_vertex == 'c') {
            return &C;
        }
        else {
            std::cout << "There is no such vertex for current four bar linkage" << std::endl;
            return 0;
        }
    }
};
class Box {
private:
    int hold_cube; // if 0 move by collision only in x, if 1 move with grip in x and z, if 2 no movement
    Point a;
    Point b;
    Point c;
    Point d;
    float cube_lenght, cube_height, cube_angle, robot_angle_before_attachment, robot_actual_angle;
public:
    Box(const Box& to_copy) : hold_cube(0) {
        (*this).Box_Get_Adress(1, this);
        (*this).a = to_copy.a;
        (*this).b = to_copy.b;
        (*this).c = to_copy.c;
        (*this).d = to_copy.d;
        (*this).cube_lenght = to_copy.cube_lenght;
        (*this).cube_height = to_copy.cube_height;
        (*this).cube_angle = to_copy.cube_angle;
        (*this).robot_angle_before_attachment = to_copy.robot_angle_before_attachment;
        (*this).robot_actual_angle = to_copy.robot_actual_angle;
    }
    Box(const float inicial_x, const float lenght, const float height, const float init_angle) : hold_cube(0), cube_lenght(lenght), cube_height(height), cube_angle(init_angle), robot_angle_before_attachment(0.0), robot_actual_angle(0.0) {
        (*this).Box_Get_Adress(1, this);
        a.set_point(inicial_x, 0);
        b.set_point(inicial_x + lenght, 0);
        c.set_point(inicial_x, height);
        d.set_point(inicial_x + lenght, height);
    }
    Box(const float inicial_x, const float lenght, const float height) : hold_cube(0), cube_lenght(lenght), cube_height(height), cube_angle(0.0), robot_angle_before_attachment(0.0), robot_actual_angle(0.0) {
        (*this).Box_Get_Adress(1, this);
        a.set_point(inicial_x, 0);
        b.set_point(inicial_x + lenght, 0);
        c.set_point(inicial_x, height);
        d.set_point(inicial_x + lenght, height);
    }
    Box(const float inicial_x, const float cube_side) : hold_cube(0), cube_lenght(cube_side), cube_height(cube_side), cube_angle(0.0), robot_angle_before_attachment(0.0), robot_actual_angle(0.0) {
        (*this).Box_Get_Adress(1, this);
        a.set_point(inicial_x, 0);
        b.set_point(inicial_x + cube_side, 0);
        c.set_point(inicial_x, cube_side);
        d.set_point(inicial_x + cube_side, cube_side);
        std::cout << "Adreess of cube: " << this << std::endl;
    }
    ~Box() {
        (*this).Box_Get_Counting(0);
#ifdef _DEBUG 
        std::cout << "Box was destructed" << std::endl;
#endif      
    }

    float box_Get_Size(const char which_size) {
        if (which_size == 'l') return cube_lenght;
        if (which_size == 'h') return cube_height;
        else return 0;
    }
    int cube_hold_status() {
        return hold_cube;
    }
    void box_de_attach(const char& de_or_a_ttach) {
        if (de_or_a_ttach == 'p') hold_cube++;
        if (de_or_a_ttach == 'm') hold_cube--;
    }
    void mov_box_coordinates(Box& const current_box, float move_x, float move_y) {
        current_box.a.set_point(current_box.a.get_point_x() + move_x * cos((*this).box_get_robot_actual_angle()) + move_y * sin((*this).box_get_robot_actual_angle()), current_box.a.get_point_y() - move_x * sin((*this).box_get_robot_actual_angle()) + move_y * cos((*this).box_get_robot_actual_angle()));
        current_box.b.set_point(current_box.b.get_point_x() + move_x * cos((*this).box_get_robot_actual_angle()) + move_y * sin((*this).box_get_robot_actual_angle()), current_box.b.get_point_y() - move_x * sin((*this).box_get_robot_actual_angle()) + move_y * cos((*this).box_get_robot_actual_angle()));
        current_box.c.set_point(current_box.c.get_point_x() + move_x * cos((*this).box_get_robot_actual_angle()) + move_y * sin((*this).box_get_robot_actual_angle()), current_box.c.get_point_y() - move_x * sin((*this).box_get_robot_actual_angle()) + move_y * cos((*this).box_get_robot_actual_angle()));
        current_box.d.set_point(current_box.d.get_point_x() + move_x * cos((*this).box_get_robot_actual_angle()) + move_y * sin((*this).box_get_robot_actual_angle()), current_box.d.get_point_y() - move_x * sin((*this).box_get_robot_actual_angle()) + move_y * cos((*this).box_get_robot_actual_angle()));
    }
    void angle_attached_box(float xz_angle_robot, Point& around_point) {
        float temp_theta = 0.0;       
        temp_theta = xz_angle_robot - (*this).box_get_robot_angle_before_attachment();
        float phi2 = temp_theta - (*this).box_get_angle();
        (*this).box_set_angle(temp_theta);      
 }
    void box_turn_back() {       
        if ((*this).box_get_angle() > 0.0f) {
            Point temp = (*this).a;
            (*this).a.set_point(((*this).a.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).a.get_point_y() * sin((*this).box_get_angle()), ((*this).a.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).a.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).a.set_point((*this).a.get_point_x() + temp.get_point_x(), (*this).a.get_point_y() + temp.get_point_y());
            (*this).b.set_point(((*this).b.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).b.get_point_y() * sin((*this).box_get_angle()), ((*this).b.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).b.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).b.set_point((*this).b.get_point_x() + temp.get_point_x(), (*this).b.get_point_y() + temp.get_point_y());
            (*this).c.set_point(((*this).c.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).c.get_point_y() * sin((*this).box_get_angle()), ((*this).c.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).c.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).c.set_point((*this).c.get_point_x() + temp.get_point_x(), (*this).c.get_point_y() + temp.get_point_y());
            (*this).d.set_point(((*this).d.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).d.get_point_y() * sin((*this).box_get_angle()), ((*this).d.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).d.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).d.set_point((*this).d.get_point_x() + temp.get_point_x(), (*this).d.get_point_y() + temp.get_point_y());
        }
        if ((*this).box_get_angle() < 0.0f) {
            Point temp = (*this).b;
            (*this).a.set_point(((*this).a.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).a.get_point_y() * sin((*this).box_get_angle()), ((*this).a.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).a.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).a.set_point((*this).a.get_point_x() + temp.get_point_x(), (*this).a.get_point_y() + temp.get_point_y());
            (*this).b.set_point(((*this).b.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).b.get_point_y() * sin((*this).box_get_angle()), ((*this).b.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).b.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).b.set_point((*this).b.get_point_x() + temp.get_point_x(), (*this).b.get_point_y() + temp.get_point_y());
            (*this).c.set_point(((*this).c.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).c.get_point_y() * sin((*this).box_get_angle()), ((*this).c.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).c.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).c.set_point((*this).c.get_point_x() + temp.get_point_x(), (*this).c.get_point_y() + temp.get_point_y());
            (*this).d.set_point(((*this).d.get_point_x() - temp.get_point_x()) * cos((*this).box_get_angle()) + (*this).d.get_point_y() * sin((*this).box_get_angle()), ((*this).d.get_point_x() - temp.get_point_x()) * (-sin((*this).box_get_angle())) + ((*this).d.get_point_y() - temp.get_point_y()) * cos((*this).box_get_angle()));
            (*this).d.set_point((*this).d.get_point_x() + temp.get_point_x(), (*this).d.get_point_y() + temp.get_point_y());
        }
        (*this).box_set_angle(0.0);
    }
    void box_set_robot_actual_angle(float phia) {
        (*this).robot_actual_angle = phia;
    }
    const float box_get_robot_actual_angle() {
        return (*this).robot_actual_angle;
    }
    void box_set_robot_angle_before_attachment(float phia) {
        (*this).robot_angle_before_attachment = phia;
    }
    const float box_get_robot_angle_before_attachment() {
        return (*this).robot_angle_before_attachment;
    }
    void box_set_angle(float set_angle) {
        (*this).cube_angle = set_angle;
    }
    const float box_get_angle() {
        return (*this).cube_angle;
    }
    static int Box_Get_Counting(int count) {
        static int number_of_cubes = 0;
        if (count == 1) number_of_cubes++;
        if (count == 0) number_of_cubes--;
        else return number_of_cubes;
    }
    const static std::vector <Box*> Box_Get_Adress(int count, Box* adress_to_copy) {
        static int number_of_cubes = 0;
        static std::vector <Box*> list_of_cubes;
        Box_Get_Counting(count);
        if (count == 1 || count == 0) {
            list_of_cubes.push_back(adress_to_copy);
        }
        if (count == 1) number_of_cubes++;
        if (count == 0) number_of_cubes--;
        else return list_of_cubes;
    }
    void box_set_point(const char& current_vertex, const float& x_coord, const float& y_coord) {
        if (current_vertex == 'a') {
            a.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'b') {
            b.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'c') {
            c.set_point(x_coord, y_coord);
        }
        else if (current_vertex == 'd') {
            d.set_point(x_coord, y_coord);
        }
        else {
            std::cout << "There is no such vertex for current cube" << std::endl;
        }
    }
    Point* box_get_point(const char& current_vertex) {
        if (current_vertex == 'a') {
            return &a;
        }
        else if (current_vertex == 'b') {
            return &b;
        }
        else if (current_vertex == 'c') {
            return &c;
        }
        else if (current_vertex == 'd') {
            return &d;
        }
        else {
            std::cout << "There is no such vertex for current four bar linkage" << std::endl;
            return 0;
        }
    }
    void print_cube_coordinates() const {
        std::cout << "Coordinates of point a in the cube: " << " x coordinate:  " << a.get_point_x() << " y coordinate:  " << a.get_point_y() << std::endl;
        std::cout << "Coordinates of point b in the cube: " << " x coordinate:  " << b.get_point_x() << " y coordinate:  " << b.get_point_y() << std::endl;
        std::cout << "Coordinates of point c in the cube: " << " x coordinate:  " << c.get_point_x() << " y coordinate:  " << c.get_point_y() << std::endl;
        std::cout << "Coordinates of point d in the cube: " << " x coordinate:  " << d.get_point_x() << " y coordinate:  " << d.get_point_y() << std::endl;
    }
};
class Grip : public Triangle {
private:
    bool grip_mode;
    Point internal_collision;
    float lk, ll;
    Box* holded_box;
    friend class Robot_arm;
public:
    Grip(const bool& grip, const Point& collide, const float& angle, const Point& p1, const Point& p2, const Point& p3) : grip_mode(grip), internal_collision(collide), lk(0), ll(0), holded_box(0), Triangle(angle, p1, p2, p3) {
    }
    Grip(const bool& grip, const Point& collide) : grip_mode(grip), internal_collision(collide), lk(0), ll(0), holded_box(0){
    }
    Grip() {
        grip_mode = 0;
        lk = 0;
        ll = 0;
        holded_box = 0;
    }
    ~Grip() {
#ifdef _DEBUG 
        std::cout << "Grip was destructed " << std::endl;
#endif 
    }
    bool is_allowed_to_move() {
        if ((*this).holded_box == nullptr) return 1;
        else if ((*this).holded_box->cube_hold_status() < 2) return 1;
        else return 0;
    }
    void grip_set_point(const char& current_vertex, const float& x_coord, const float& y_coord) {
        if (current_vertex == 'a' || current_vertex == 'b' || current_vertex == 'c') {
            (*this).triangle_set_point(current_vertex, x_coord, y_coord);
        }
        else if (current_vertex == 'd') {
            internal_collision.set_point(x_coord, y_coord);
        }
        else {
            std::cout << "There is no such vertex for current grip" << std::endl;
        }
    }
    void set_grip_points(const float& phi, const Point& O_1, const Point& O_2, const float& a, const float& b, bool is_mirrorred) {
        //phi is the angle between point C and the ground, a is the link from point A to point C, b is the collision point
        (*this).set_triangle_points(phi, O_1, O_2, a);
        (*this).lk = a;
        (*this).ll = b;
        float temp_mov_x = 0, temp_mov_y = 0;
        temp_mov_x = -(*this).internal_collision.get_point_x();
        temp_mov_y = -(*this).internal_collision.get_point_y();
        if ((*this).grip_mode) {
            internal_collision.set_point(O_1.get_point_x() + (ll * cos(phi)), O_1.get_point_y() + (ll * sin(phi)));
        }
        else if (!(*this).grip_mode) {
            internal_collision.set_point(O_1.get_point_x() + (lk * cos(phi)), O_1.get_point_y() + (lk * sin(phi)));
        }
        temp_mov_x = temp_mov_x + (*this).internal_collision.get_point_x();
        temp_mov_y = temp_mov_y + (*this).internal_collision.get_point_y();
        if ((*this).holded_box) {
            if (is_mirrorred) {
                (*this).holded_box->mov_box_coordinates(*(*this).holded_box, -temp_mov_x, temp_mov_y);
            } else {
                (*this).holded_box->mov_box_coordinates(*(*this).holded_box, temp_mov_x, temp_mov_y);
            }
        }
    }




    void change_grip_status(float robot_ground_angle_before_attachment) {
        grip_mode = !grip_mode;
        if (grip_mode) {
            //Release when possible
            internal_collision.set_point((*this).triangle_get_point('a')->get_point_x() + (ll * cos((*this).get_triangle_angle() - robot_ground_angle_before_attachment)), (*this).triangle_get_point('a')->get_point_y() + (ll * sin((*this).get_triangle_angle() - robot_ground_angle_before_attachment)));
             if (holded_box) {
                holded_box->box_set_robot_angle_before_attachment(0.0);
                float d_ground = holded_box->box_get_point('a')->get_point_y();
                if (d_ground > holded_box->box_get_point('b')->get_point_y()) {
                    d_ground = holded_box->box_get_point('b')->get_point_y();
                }
                holded_box->box_de_attach('m');
                holded_box->box_set_point('a', holded_box->box_get_point('a')->get_point_x(), holded_box->box_get_point('a')->get_point_y() - d_ground);
                holded_box->box_set_point('b', holded_box->box_get_point('b')->get_point_x(), holded_box->box_get_point('b')->get_point_y() - d_ground);
                holded_box->box_set_point('c', holded_box->box_get_point('c')->get_point_x(), holded_box->box_get_point('c')->get_point_y() - d_ground);
                holded_box->box_set_point('d', holded_box->box_get_point('d')->get_point_x(), holded_box->box_get_point('d')->get_point_y() - d_ground);
                holded_box->box_turn_back();
            }
            holded_box = nullptr;
        }
        else if (!grip_mode) {
            //Attach when possible
            internal_collision.set_point((*this).triangle_get_point('a')->get_point_x() + (lk * cos((*this).get_triangle_angle() - robot_ground_angle_before_attachment)), (*this).triangle_get_point('a')->get_point_y() + (lk * sin((*this).get_triangle_angle() - robot_ground_angle_before_attachment)));
            for (int i = 0; i < Box::Box_Get_Adress(2, 0).size(); i++) {



                if ((*this).grip_collision_point()->get_point_x() > Box::Box_Get_Adress(2, 0)[i]->box_get_point('a')->get_point_x() && (*this).grip_collision_point()->get_point_x() < Box::Box_Get_Adress(2, 0)[i]->box_get_point('d')->get_point_x() && (*this).grip_collision_point()->get_point_y() > Box::Box_Get_Adress(2, 0)[i]->box_get_point('a')->get_point_y() && (*this).grip_collision_point()->get_point_y() < Box::Box_Get_Adress(2, 0)[i]->box_get_point('d')->get_point_y()) {
                    (*this).holded_box = Box::Box_Get_Adress(2, 0)[i];
                    (*this).holded_box->box_de_attach('p');
                    (*this).holded_box->box_set_robot_angle_before_attachment(-robot_ground_angle_before_attachment);
                    (*this).holded_box->box_set_robot_actual_angle(robot_ground_angle_before_attachment);




                }
            }
        }
    }

    /*
        public function triangleArea(A:Point,B:Point,C:Point):Number {
			return (C.x*B.y-B.x*C.y)-(C.x*A.y-A.x*C.y)+(B.x*A.y-A.x*B.y);
		}
		public function isInsideSquare(A:Point,B:Point,C:Point,D:Point,P:Point):Boolean {
			if (triangleArea(A,B,P)>0 || triangleArea(B,C,P)>0 || triangleArea(C,D,P)>0 || triangleArea(D,A,P)>0) {
				return false;
			}
			return true;
        }
    */




    void change_grip_status_while_mirrorred(float robot_ground_angle_before_attachment) {
        grip_mode = !grip_mode;
        if (grip_mode) {
            //Release when possible
            internal_collision.set_point((*this).triangle_get_point('a')->get_point_x() + (-ll * cos((*this).get_triangle_angle() + robot_ground_angle_before_attachment)), (*this).triangle_get_point('a')->get_point_y() + (ll * sin((*this).get_triangle_angle() + robot_ground_angle_before_attachment)));
            if (holded_box) {
                holded_box->box_set_robot_angle_before_attachment(0.0);
                float d_ground = holded_box->box_get_point('a')->get_point_y();
                if (d_ground > holded_box->box_get_point('b')->get_point_y()) {
                    d_ground = holded_box->box_get_point('b')->get_point_y();
                }
                holded_box->box_de_attach('m');
                holded_box->box_set_point('a', holded_box->box_get_point('a')->get_point_x(), holded_box->box_get_point('a')->get_point_y() - d_ground);
                holded_box->box_set_point('b', holded_box->box_get_point('b')->get_point_x(), holded_box->box_get_point('b')->get_point_y() - d_ground);
                holded_box->box_set_point('c', holded_box->box_get_point('c')->get_point_x(), holded_box->box_get_point('c')->get_point_y() - d_ground);
                holded_box->box_set_point('d', holded_box->box_get_point('d')->get_point_x(), holded_box->box_get_point('d')->get_point_y() - d_ground);
                holded_box->box_turn_back();
            }
            holded_box = nullptr;
        }
        else if (!grip_mode) {
            
            
            
            
            
            //Attach when possible
            internal_collision.set_point((*this).triangle_get_point('a')->get_point_x() + (-lk * cos((*this).get_triangle_angle() + robot_ground_angle_before_attachment)), (*this).triangle_get_point('a')->get_point_y() + (lk * sin((*this).get_triangle_angle() + robot_ground_angle_before_attachment)));
            for (int i = 0; i < Box::Box_Get_Adress(2, 0).size(); i++) {
                if ((*this).grip_collision_point()->get_point_x() > Box::Box_Get_Adress(2, 0)[i]->box_get_point('a')->get_point_x() && (*this).grip_collision_point()->get_point_x() < Box::Box_Get_Adress(2, 0)[i]->box_get_point('d')->get_point_x() && (*this).grip_collision_point()->get_point_y() > Box::Box_Get_Adress(2, 0)[i]->box_get_point('a')->get_point_y() && (*this).grip_collision_point()->get_point_y() < Box::Box_Get_Adress(2, 0)[i]->box_get_point('d')->get_point_y()) {
                    (*this).holded_box = Box::Box_Get_Adress(2, 0)[i];
                    (*this).holded_box->box_de_attach('p');
                    (*this).holded_box->box_set_robot_angle_before_attachment(-robot_ground_angle_before_attachment);
                    (*this).holded_box->box_set_robot_actual_angle(robot_ground_angle_before_attachment);
                }


                /*
                public function triangleArea(A:Point,B:Point,C:Point):Number {
			        return (C.x*B.y-B.x*C.y)-(C.x*A.y-A.x*C.y)+(B.x*A.y-A.x*B.y);
		        }
		        public function isInsideSquare(A:Point,B:Point,C:Point,D:Point,P:Point):Boolean {
			        if (triangleArea(A,B,P)>0 || triangleArea(B,C,P)>0 || triangleArea(C,D,P)>0 || triangleArea(D,A,P)>0) {
				        return false;
			        }
			        return true;
		        }
                */




            }





        }
    }
    void print_grip_coordinates() const {
        std::cout << "Grip mode: " << grip_mode << std::endl;
        std::cout << "Coordinates of internal collision point in the triangle: " << " x coordinate:  " << internal_collision.get_point_x() << " y coordinate:  " << internal_collision.get_point_y() << std::endl;
        (*this).print_triangle_coordinates();
    };
    const Point* grip_collision_point() const {
        return &internal_collision;
    }
};
class Robot_arm {
private:
    bool is_mirrorred;
    Point origin;
    float Initial_robot_x, Initial_robot_y, angle_before_grip_closes, xz_angle, theta_M1_max, theta_M1_min, theta_M2_max, theta_M2_min, le, lc, la, lf, li, ld, lh, lg, lb, lj, lk, theta1, theta37, theta810, theta_M1, theta_M2, forward_max_contact, backward_max_contact, Maximum_angle_long_arm;
    mutable float theta_M1_searched;
    mutable float theta_M2_searched;
    Four_bar_linkage backward_mechanism;
    Four_bar_linkage upward_mechanism;
    Four_bar_linkage driven_mechanism;
    Triangle linkage_triangle;
    Grip end_grip;

    void reset_search() {
        (*this).theta_M1_searched = 0;
        (*this).theta_M2_searched = 0;
    }
    void mirror_robot_arm() {
        float mirror_around;
        mirror_around = (*this).backward_mechanism.four_bar_get_point('a')->get_point_x();
        (*this).backward_mechanism.four_bar_set_point('a', (2 * mirror_around - (*this).backward_mechanism.four_bar_get_point('a')->get_point_x()), (*this).backward_mechanism.four_bar_get_point('a')->get_point_y());
        (*this).backward_mechanism.four_bar_set_point('b', (2 * mirror_around - (*this).backward_mechanism.four_bar_get_point('b')->get_point_x()), (*this).backward_mechanism.four_bar_get_point('b')->get_point_y());
        (*this).backward_mechanism.four_bar_set_point('c', (2 * mirror_around - (*this).backward_mechanism.four_bar_get_point('c')->get_point_x()), (*this).backward_mechanism.four_bar_get_point('c')->get_point_y());
        (*this).backward_mechanism.four_bar_set_point('d', (2 * mirror_around - (*this).backward_mechanism.four_bar_get_point('d')->get_point_x()), (*this).backward_mechanism.four_bar_get_point('d')->get_point_y());
        (*this).upward_mechanism.four_bar_set_point('a', (2 * mirror_around - (*this).upward_mechanism.four_bar_get_point('a')->get_point_x()), (*this).upward_mechanism.four_bar_get_point('a')->get_point_y());
        (*this).upward_mechanism.four_bar_set_point('b', (2 * mirror_around - (*this).upward_mechanism.four_bar_get_point('b')->get_point_x()), (*this).upward_mechanism.four_bar_get_point('b')->get_point_y());
        (*this).upward_mechanism.four_bar_set_point('c', (2 * mirror_around - (*this).upward_mechanism.four_bar_get_point('c')->get_point_x()), (*this).upward_mechanism.four_bar_get_point('c')->get_point_y());
        (*this).upward_mechanism.four_bar_set_point('d', (2 * mirror_around - (*this).upward_mechanism.four_bar_get_point('d')->get_point_x()), (*this).upward_mechanism.four_bar_get_point('d')->get_point_y());
        (*this).linkage_triangle.triangle_set_point('a', (2 * mirror_around - (*this).linkage_triangle.triangle_get_point('a')->get_point_x()), (*this).linkage_triangle.triangle_get_point('a')->get_point_y());
        (*this).linkage_triangle.triangle_set_point('b', (2 * mirror_around - (*this).linkage_triangle.triangle_get_point('b')->get_point_x()), (*this).linkage_triangle.triangle_get_point('b')->get_point_y());
        (*this).linkage_triangle.triangle_set_point('c', (2 * mirror_around - (*this).linkage_triangle.triangle_get_point('c')->get_point_x()), (*this).linkage_triangle.triangle_get_point('c')->get_point_y());
        (*this).driven_mechanism.four_bar_set_point('a', (2 * mirror_around - (*this).driven_mechanism.four_bar_get_point('a')->get_point_x()), (*this).driven_mechanism.four_bar_get_point('a')->get_point_y());
        (*this).driven_mechanism.four_bar_set_point('b', (2 * mirror_around - (*this).driven_mechanism.four_bar_get_point('b')->get_point_x()), (*this).driven_mechanism.four_bar_get_point('b')->get_point_y());
        (*this).driven_mechanism.four_bar_set_point('c', (2 * mirror_around - (*this).driven_mechanism.four_bar_get_point('c')->get_point_x()), (*this).driven_mechanism.four_bar_get_point('c')->get_point_y());
        (*this).driven_mechanism.four_bar_set_point('d', (2 * mirror_around - (*this).driven_mechanism.four_bar_get_point('d')->get_point_x()), (*this).driven_mechanism.four_bar_get_point('d')->get_point_y());
        (*this).end_grip.grip_set_point('a', (2 * mirror_around - (*this).end_grip.triangle_get_point('a')->get_point_x()), (*this).end_grip.triangle_get_point('a')->get_point_y());
        (*this).end_grip.grip_set_point('b', (2 * mirror_around - (*this).end_grip.triangle_get_point('b')->get_point_x()), (*this).end_grip.triangle_get_point('b')->get_point_y());
        (*this).end_grip.grip_set_point('c', (2 * mirror_around - (*this).end_grip.triangle_get_point('c')->get_point_x()), (*this).end_grip.triangle_get_point('c')->get_point_y());
        (*this).end_grip.grip_set_point('d', (2 * mirror_around - (*this).end_grip.grip_collision_point()->get_point_x()), (*this).end_grip.grip_collision_point()->get_point_y());
    }
    void angle_robot_arm(float phi, bool to_move) {
        float temp_x = (*this).backward_mechanism.four_bar_get_point('a')->get_point_x(), temp_y = (*this).backward_mechanism.four_bar_get_point('a')->get_point_y();
        (*this).backward_mechanism.four_bar_set_point('a', ((*this).backward_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * cos(phi) + ((*this).backward_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * sin(phi), ((*this).backward_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).backward_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * cos(phi));
        (*this).backward_mechanism.four_bar_set_point('a', (*this).backward_mechanism.four_bar_get_point('a')->get_point_x() + temp_x, (*this).backward_mechanism.four_bar_get_point('a')->get_point_y() + temp_y);
        (*this).backward_mechanism.four_bar_set_point('b', ((*this).backward_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * cos(phi) + ((*this).backward_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * sin(phi), ((*this).backward_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).backward_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * cos(phi));
        (*this).backward_mechanism.four_bar_set_point('b', (*this).backward_mechanism.four_bar_get_point('b')->get_point_x() + temp_x, (*this).backward_mechanism.four_bar_get_point('b')->get_point_y() + temp_y);
        (*this).backward_mechanism.four_bar_set_point('d', ((*this).backward_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * cos(phi) + ((*this).backward_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * sin(phi), ((*this).backward_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).backward_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * cos(phi));
        (*this).backward_mechanism.four_bar_set_point('d', (*this).backward_mechanism.four_bar_get_point('d')->get_point_x() + temp_x, (*this).backward_mechanism.four_bar_get_point('d')->get_point_y() + temp_y);
        (*this).backward_mechanism.four_bar_set_point('c', ((*this).backward_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * cos(phi) + ((*this).backward_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * sin(phi), ((*this).backward_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).backward_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * cos(phi));
        (*this).backward_mechanism.four_bar_set_point('c', (*this).backward_mechanism.four_bar_get_point('c')->get_point_x() + temp_x, (*this).backward_mechanism.four_bar_get_point('c')->get_point_y() + temp_y);
        (*this).upward_mechanism.four_bar_set_point('a', ((*this).upward_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * cos(phi) + ((*this).upward_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * sin(phi), ((*this).upward_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).upward_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * cos(phi));
        (*this).upward_mechanism.four_bar_set_point('a', (*this).upward_mechanism.four_bar_get_point('a')->get_point_x() + temp_x, (*this).upward_mechanism.four_bar_get_point('a')->get_point_y() + temp_y);
        (*this).upward_mechanism.four_bar_set_point('b', ((*this).upward_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * cos(phi) + ((*this).upward_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * sin(phi), ((*this).upward_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).upward_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * cos(phi));
        (*this).upward_mechanism.four_bar_set_point('b', (*this).upward_mechanism.four_bar_get_point('b')->get_point_x() + temp_x, (*this).upward_mechanism.four_bar_get_point('b')->get_point_y() + temp_y);
        (*this).upward_mechanism.four_bar_set_point('d', ((*this).upward_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * cos(phi) + ((*this).upward_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * sin(phi), ((*this).upward_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).upward_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * cos(phi));
        (*this).upward_mechanism.four_bar_set_point('d', (*this).upward_mechanism.four_bar_get_point('d')->get_point_x() + temp_x, (*this).upward_mechanism.four_bar_get_point('d')->get_point_y() + temp_y);
        (*this).upward_mechanism.four_bar_set_point('c', ((*this).upward_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * cos(phi) + ((*this).upward_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * sin(phi), ((*this).upward_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).upward_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * cos(phi));
        (*this).upward_mechanism.four_bar_set_point('c', (*this).upward_mechanism.four_bar_get_point('c')->get_point_x() + temp_x, (*this).upward_mechanism.four_bar_get_point('c')->get_point_y() + temp_y);
        (*this).linkage_triangle.triangle_set_point('a', ((*this).linkage_triangle.triangle_get_point('a')->get_point_x() - temp_x) * cos(phi) + ((*this).linkage_triangle.triangle_get_point('a')->get_point_y() - temp_y) * sin(phi), ((*this).linkage_triangle.triangle_get_point('a')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).linkage_triangle.triangle_get_point('a')->get_point_y() - temp_y) * cos(phi));
        (*this).linkage_triangle.triangle_set_point('a', (*this).linkage_triangle.triangle_get_point('a')->get_point_x() + temp_x, (*this).linkage_triangle.triangle_get_point('a')->get_point_y() + temp_y);
        (*this).linkage_triangle.triangle_set_point('b', ((*this).linkage_triangle.triangle_get_point('b')->get_point_x() - temp_x) * cos(phi) + ((*this).linkage_triangle.triangle_get_point('b')->get_point_y() - temp_y) * sin(phi), ((*this).linkage_triangle.triangle_get_point('b')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).linkage_triangle.triangle_get_point('b')->get_point_y() - temp_y) * cos(phi));
        (*this).linkage_triangle.triangle_set_point('b', (*this).linkage_triangle.triangle_get_point('b')->get_point_x() + temp_x, (*this).linkage_triangle.triangle_get_point('b')->get_point_y() + temp_y);
        (*this).linkage_triangle.triangle_set_point('c', ((*this).linkage_triangle.triangle_get_point('c')->get_point_x() - temp_x) * cos(phi) + ((*this).linkage_triangle.triangle_get_point('c')->get_point_y() - temp_y) * sin(phi), ((*this).linkage_triangle.triangle_get_point('c')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).linkage_triangle.triangle_get_point('c')->get_point_y() - temp_y) * cos(phi));
        (*this).linkage_triangle.triangle_set_point('c', (*this).linkage_triangle.triangle_get_point('c')->get_point_x() + temp_x, (*this).linkage_triangle.triangle_get_point('c')->get_point_y() + temp_y);
        (*this).driven_mechanism.four_bar_set_point('a', ((*this).driven_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * cos(phi) + ((*this).driven_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * sin(phi), ((*this).driven_mechanism.four_bar_get_point('a')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).driven_mechanism.four_bar_get_point('a')->get_point_y() - temp_y) * cos(phi));
        (*this).driven_mechanism.four_bar_set_point('a', (*this).driven_mechanism.four_bar_get_point('a')->get_point_x() + temp_x, (*this).driven_mechanism.four_bar_get_point('a')->get_point_y() + temp_y);
        (*this).driven_mechanism.four_bar_set_point('b', ((*this).driven_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * cos(phi) + ((*this).driven_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * sin(phi), ((*this).driven_mechanism.four_bar_get_point('b')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).driven_mechanism.four_bar_get_point('b')->get_point_y() - temp_y) * cos(phi));
        (*this).driven_mechanism.four_bar_set_point('b', (*this).driven_mechanism.four_bar_get_point('b')->get_point_x() + temp_x, (*this).driven_mechanism.four_bar_get_point('b')->get_point_y() + temp_y);
        (*this).driven_mechanism.four_bar_set_point('d', ((*this).driven_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * cos(phi) + ((*this).driven_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * sin(phi), ((*this).driven_mechanism.four_bar_get_point('d')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).driven_mechanism.four_bar_get_point('d')->get_point_y() - temp_y) * cos(phi));
        (*this).driven_mechanism.four_bar_set_point('d', (*this).driven_mechanism.four_bar_get_point('d')->get_point_x() + temp_x, (*this).driven_mechanism.four_bar_get_point('d')->get_point_y() + temp_y);
        (*this).driven_mechanism.four_bar_set_point('c', ((*this).driven_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * cos(phi) + ((*this).driven_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * sin(phi), ((*this).driven_mechanism.four_bar_get_point('c')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).driven_mechanism.four_bar_get_point('c')->get_point_y() - temp_y) * cos(phi));
        (*this).driven_mechanism.four_bar_set_point('c', (*this).driven_mechanism.four_bar_get_point('c')->get_point_x() + temp_x, (*this).driven_mechanism.four_bar_get_point('c')->get_point_y() + temp_y);
        (*this).end_grip.grip_set_point('a', ((*this).end_grip.triangle_get_point('a')->get_point_x() - temp_x) * cos(phi) + ((*this).end_grip.triangle_get_point('a')->get_point_y() - temp_y) * sin(phi), ((*this).end_grip.triangle_get_point('a')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).end_grip.triangle_get_point('a')->get_point_y() - temp_y) * cos(phi));
        (*this).end_grip.grip_set_point('a', (*this).end_grip.triangle_get_point('a')->get_point_x() + temp_x, (*this).end_grip.triangle_get_point('a')->get_point_y() + temp_y);
        (*this).end_grip.grip_set_point('b', ((*this).end_grip.triangle_get_point('b')->get_point_x() - temp_x) * cos(phi) + ((*this).end_grip.triangle_get_point('b')->get_point_y() - temp_y) * sin(phi), ((*this).end_grip.triangle_get_point('b')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).end_grip.triangle_get_point('b')->get_point_y() - temp_y) * cos(phi));
        (*this).end_grip.grip_set_point('b', (*this).end_grip.triangle_get_point('b')->get_point_x() + temp_x, (*this).end_grip.triangle_get_point('b')->get_point_y() + temp_y);
        (*this).end_grip.grip_set_point('c', ((*this).end_grip.triangle_get_point('c')->get_point_x() - temp_x) * cos(phi) + ((*this).end_grip.triangle_get_point('c')->get_point_y() - temp_y) * sin(phi), ((*this).end_grip.triangle_get_point('c')->get_point_x() - temp_x) * (-sin(phi)) + ((*this).end_grip.triangle_get_point('c')->get_point_y() - temp_y) * cos(phi));
        (*this).end_grip.grip_set_point('c', (*this).end_grip.triangle_get_point('c')->get_point_x() + temp_x, (*this).end_grip.triangle_get_point('c')->get_point_y() + temp_y);
        (*this).end_grip.grip_set_point('d', ((*this).end_grip.grip_collision_point()->get_point_x() - temp_x) * cos(phi) + ((*this).end_grip.grip_collision_point()->get_point_y() - temp_y) * sin(phi), ((*this).end_grip.grip_collision_point()->get_point_x() - temp_x) * (-sin(phi)) + ((*this).end_grip.grip_collision_point()->get_point_y() - temp_y) * cos(phi));
        (*this).end_grip.grip_set_point('d', (*this).end_grip.grip_collision_point()->get_point_x() + temp_x, (*this).end_grip.grip_collision_point()->get_point_y() + temp_y);
        if ((*this).end_grip.holded_box && to_move) {
            if ((*this).xz_angle != 0.0f){
                (*this).end_grip.holded_box->angle_attached_box(-phi, (*this).origin);
            }
        }
    }

public:
    Robot_arm(const Robot_arm& to_copy) : Initial_robot_x(to_copy.Initial_robot_x), Initial_robot_y(to_copy.Initial_robot_y), angle_before_grip_closes(0.0), is_mirrorred(to_copy.is_mirrorred), origin(to_copy.origin), xz_angle(to_copy.xz_angle), theta_M1_max(to_copy.theta_M1_max), theta_M1_min(to_copy.theta_M1_min), theta_M2_max(to_copy.theta_M2_max), theta_M2_min(to_copy.theta_M2_min), le(to_copy.le), lc(to_copy.lc), la(to_copy.la), lf(to_copy.lf), li(to_copy.li), ld(to_copy.ld), lh(to_copy.lh), lg(to_copy.lg), lb(to_copy.lb), lj(to_copy.lj), lk(to_copy.lk), theta1(to_copy.theta1), theta37(to_copy.theta37), theta810(to_copy.theta810), theta_M1(to_copy.theta_M1), theta_M2(to_copy.theta_M2), theta_M1_searched(0), theta_M2_searched(0), forward_max_contact(to_copy.forward_max_contact), backward_max_contact(to_copy.backward_max_contact), Maximum_angle_long_arm(to_copy.Maximum_angle_long_arm) {
        (*this).Robot_Get_Counting(1);
        (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
    }
    Robot_arm() {
        Initial_robot_x = 35.0f;
        Initial_robot_y = 25.763f;
        (*this).Robot_Get_Counting(1);
        is_mirrorred = false;
        xz_angle = 0.0f;
        origin.set_point(0.0, 0.0);
        theta_M1_searched = 0.0f;
        theta_M2_searched = 0.0f;
        angle_before_grip_closes = 0.0;
        //Adjust this values to the robot arm
        //Maximum and minimum angles for the motors
        //Degrees to radians (* 3.14159265 / 180.0)
        theta_M1_max = (float)(145 * 3.14159265 / 180.0);
        theta_M1_min = (float)(0 * 3.14159265 / 180.0);
        theta_M2_max = (float)(220 * 3.14159265 / 180.0);
        theta_M2_min = (float)(110 * 3.14159265 / 180.0);

        //Lenght of each link
        le = 35;      //vertical drive arm
        lc = 80;      //link
        la = 80;      //forward drive arm
        lf = 35;      //left piece of horizontal arm
        li = 80;      //right piece of horizontal arm
        ld = 40;      //left part of triangular link
        lh = 20;      //right part of triangular link
        lg = 46.25;   //upper part of triangular link
        lb = 40;      //link attached to base
        lj = 20;      //grid attachment link
        lk = 45.2355; //link to point of griping

        // Predefined angles
        theta1 = (float)((154.5) * 3.14159265 / 180.0);        // Angle between P1and P2and ground
        theta37 = (float)(95 * 3.14159265 / 180.0);         // Angle between P4and P3and P7
        theta810 = (float)(72.89743 * 3.14159265 / 180.0);  // Angle between ljand lk

        // Initial position of motor angle
        theta_M1 = theta_M1_max;   // Engine controlling forward drive arm
        theta_M2 = theta_M2_max;   // Engine controlling vertical drive arm

        // Physical limitations
        forward_max_contact = (float)(40.78 * 3.14159265 / 180.0);    //Limitation for forward movement before internal collision
        backward_max_contact = (float)(153.2 * 3.14159265 / 180.0);   //Limitation for backward movement before internal collision
        Maximum_angle_long_arm = (float)(85.00 * 3.14159265 / 180.0); //Limitation for downward movement before internal collision

        (*this).set_robot_arm_points(theta_M1_max, theta_M2_max, false); //theta_M1_max, theta_M2_max
        (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
    }
    Robot_arm(float Initial_robot_xa, float Initial_robot_ya, const bool& mirrorred, const Point& origina, const float& xz_anglea, const float& theta_M1_maxa, const float& theta_M1_mina, const float& theta_M2_maxa, const float& theta_M2_mina, const float& lea, const float& lca, const float& laa, const float& lfa, const float& lia, const float& lda, const float& lha, const float& lga, const float& lba, const float& lja, const float& lka, const float& theta1a, const float& theta37a, const float& theta810a, const float& theta_M1a, const float& theta_M2a, const float& forward_max_contacta, const float& backward_max_contacta, const float& Maximum_angle_long_arma) : Initial_robot_x(Initial_robot_xa), Initial_robot_y(Initial_robot_ya), angle_before_grip_closes(0.0), is_mirrorred(mirrorred), origin(origina), xz_angle(xz_anglea), theta_M1_max(theta_M1_maxa), theta_M1_min(theta_M1_mina), theta_M2_max(theta_M2_maxa), theta_M2_min(theta_M2_mina), le(lea), lc(lca), la(laa), lf(lfa), li(lia), ld(lda), lh(lha), lg(lga), lb(lba), lj(lja), lk(lka), theta1(theta1a), theta37(theta37a), theta810(theta810a), theta_M1(theta_M1a), theta_M2(theta_M2a), theta_M1_searched(0), theta_M2_searched(0), forward_max_contact(forward_max_contacta), backward_max_contact(backward_max_contacta), Maximum_angle_long_arm(Maximum_angle_long_arma) {
        (*this).Robot_Get_Counting(1);
        (*this).set_robot_arm_points(theta_M1_max, theta_M2_max, false);
        (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
    }
    ~Robot_arm() {
        (*this).Robot_Get_Counting(0);
#ifdef _DEBUG 
        std::cout << "Robot arm was destructed " << std::endl;
#endif   
    }

    bool mirror_status() {
        return (*this).is_mirrorred;
    }
    bool grip_status() {
        return (*this).end_grip.grip_mode;
    }
    bool is_allowed_to_move() {
        return (*this).end_grip.is_allowed_to_move();
    }
    void change_grip_status() {
        if (!(*this).is_mirrorred) {
            angle_before_grip_closes = (*this).xz_angle;
            (*this).end_grip.change_grip_status(angle_before_grip_closes);
        }
        if ((*this).is_mirrorred) {
            angle_before_grip_closes = (*this).xz_angle;
            (*this).end_grip.change_grip_status_while_mirrorred(angle_before_grip_closes);
        }
    }
    static int Robot_Get_Counting(int count) {
        static int number_of_robot_arms = 0;
        if (count == 1) number_of_robot_arms++;
        if (count == 0) number_of_robot_arms--;
        else return number_of_robot_arms;
    }
    bool set_robot_arm_points(const float& theta_M1a, const float& theta_M2a, const bool& searching) {
        if ((*this).end_grip.is_allowed_to_move()) {
            float theta_138 = 0;
            float temp_theta_M1 = (*this).theta_M1, temp_theta_M2 = (*this).theta_M2;
            if (theta_M1a<theta_M1_min || theta_M1a>theta_M1_max || theta_M2a<theta_M2_min || theta_M2a>theta_M2_max) {
                if (!searching) std::cout << "Angle of one of the motor is out of range. Nothing to be done." << std::endl;
                return 0;
            }
            else {
                (*this).theta_M1 = theta_M1a;
                (*this).theta_M2 = theta_M2a;
                backward_mechanism.set_four_bar_points((*this).theta_M1, theta1, origin, la, lb, lc, ld);
                upward_mechanism.set_four_bar_points((*this).theta_M1, (*this).theta_M2, origin, la, le, lc, lf);
                linkage_triangle.set_triangle_points((3.14159265 - atan2((backward_mechanism.four_bar_get_point('c')->get_point_y() - backward_mechanism.four_bar_get_point('d')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('c')->get_point_x())) - theta37), *backward_mechanism.four_bar_get_point('d'), *backward_mechanism.four_bar_get_point('c'), lh);
                driven_mechanism.set_four_bar_points(atan2((backward_mechanism.four_bar_get_point('d')->get_point_y() - upward_mechanism.four_bar_get_point('c')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - upward_mechanism.four_bar_get_point('c')->get_point_x())), (3.14159265 - atan2((backward_mechanism.four_bar_get_point('c')->get_point_y() - backward_mechanism.four_bar_get_point('d')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('c')->get_point_x())) - theta37), *(backward_mechanism.four_bar_get_point('d')), li, lh, lc, lj);
                end_grip.set_grip_points(atan2((driven_mechanism.four_bar_get_point('c')->get_point_y() - driven_mechanism.four_bar_get_point('d')->get_point_y()), (driven_mechanism.four_bar_get_point('c')->get_point_x() - driven_mechanism.four_bar_get_point('d')->get_point_x())) - theta810, *driven_mechanism.four_bar_get_point('d'), *driven_mechanism.four_bar_get_point('c'), lk + 15, lk, (*this).is_mirrorred);
                theta_138 = acos(-(((backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('a')->get_point_x()) * ((driven_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('a')->get_point_x()) - (backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('a')->get_point_x()))) + (backward_mechanism.four_bar_get_point('d')->get_point_y() - backward_mechanism.four_bar_get_point('a')->get_point_y()) * ((driven_mechanism.four_bar_get_point('d')->get_point_y() - backward_mechanism.four_bar_get_point('a')->get_point_y()) - (backward_mechanism.four_bar_get_point('d')->get_point_y() - backward_mechanism.four_bar_get_point('a')->get_point_y()))) / (la * li));             
                if ((theta_138 > forward_max_contact) && (theta_138 < backward_max_contact) && ((backward_mechanism.four_bar_get_point('d')->get_point_y() < driven_mechanism.four_bar_get_point('d')->get_point_y()) || ((driven_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('d')->get_point_x()) > li * cos(Maximum_angle_long_arm)))){ //Conditions for internal collision                    
                    return 1;
                }
                else {
                    if (!searching) std::cout << "Internal collision detected. Point is out of range. Nothing to be done..." << std::endl;
                    (*this).theta_M1 = temp_theta_M1;
                    (*this).theta_M2 = temp_theta_M2;
                    backward_mechanism.set_four_bar_points((*this).theta_M1, (*this).theta1, origin, la, lb, lc, ld);
                    upward_mechanism.set_four_bar_points((*this).theta_M1, (*this).theta_M2, origin, la, le, lc, lf);
                    linkage_triangle.set_triangle_points((3.14159265 - atan2((backward_mechanism.four_bar_get_point('c')->get_point_y() - backward_mechanism.four_bar_get_point('d')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('c')->get_point_x())) - theta37), *backward_mechanism.four_bar_get_point('d'), *backward_mechanism.four_bar_get_point('c'), lh);
                    driven_mechanism.set_four_bar_points(atan2((backward_mechanism.four_bar_get_point('d')->get_point_y() - upward_mechanism.four_bar_get_point('c')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - upward_mechanism.four_bar_get_point('c')->get_point_x())), (3.14159265 - atan2((backward_mechanism.four_bar_get_point('c')->get_point_y() - backward_mechanism.four_bar_get_point('d')->get_point_y()), (backward_mechanism.four_bar_get_point('d')->get_point_x() - backward_mechanism.four_bar_get_point('c')->get_point_x())) - theta37), *(backward_mechanism.four_bar_get_point('d')), li, lh, lc, lj);
                    end_grip.set_grip_points(atan2((driven_mechanism.four_bar_get_point('c')->get_point_y() - driven_mechanism.four_bar_get_point('d')->get_point_y()), (driven_mechanism.four_bar_get_point('c')->get_point_x() - driven_mechanism.four_bar_get_point('d')->get_point_x())) - theta810, *driven_mechanism.four_bar_get_point('d'), *driven_mechanism.four_bar_get_point('c'), lk + 15, lk, (*this).is_mirrorred);
                    return 0;
                }
            }
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
            return 0;
        }
    }
    bool mov_robot_arm_by_servos(const float& plus_theta_M1a, const float& plus_theta_M2a,bool nothing) {
        if ((*this).end_grip.is_allowed_to_move()) {
            theta_M1 += plus_theta_M1a;
            theta_M2 += plus_theta_M2a;
            if ((*this).xz_angle != 0.0) (*this).angle_robot_arm(-xz_angle,false);
            if ((*this).is_mirrorred) (*this).mirror_robot_arm();
            if (!(*this).set_robot_arm_points(theta_M1, theta_M2, false)) {
                theta_M1 -= plus_theta_M1a;
                theta_M2 -= plus_theta_M2a;
                (*this).set_robot_arm_points(theta_M1, theta_M2, false);
            }
            if ((*this).is_mirrorred) (*this).mirror_robot_arm();
            if ((*this).xz_angle != 0.0) (*this).angle_robot_arm(xz_angle,false);
            return 1;
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
            return 0;
        }
    }
    void mov_robot_arm_coordinates(Robot_arm& current_robot_arm, float move_x, float move_y) {
        if ((*this).end_grip.is_allowed_to_move()) {
            current_robot_arm.backward_mechanism.four_bar_set_point('a', ((*this).backward_mechanism.four_bar_get_point('a')->get_point_x()) + move_x, (*this).backward_mechanism.four_bar_get_point('a')->get_point_y() + move_y);
            current_robot_arm.backward_mechanism.four_bar_set_point('b', ((*this).backward_mechanism.four_bar_get_point('b')->get_point_x()) + move_x, (*this).backward_mechanism.four_bar_get_point('b')->get_point_y() + move_y);
            current_robot_arm.backward_mechanism.four_bar_set_point('c', ((*this).backward_mechanism.four_bar_get_point('c')->get_point_x()) + move_x, (*this).backward_mechanism.four_bar_get_point('c')->get_point_y() + move_y);
            current_robot_arm.backward_mechanism.four_bar_set_point('d', ((*this).backward_mechanism.four_bar_get_point('d')->get_point_x()) + move_x, (*this).backward_mechanism.four_bar_get_point('d')->get_point_y() + move_y);
            current_robot_arm.upward_mechanism.four_bar_set_point('a', ((*this).upward_mechanism.four_bar_get_point('a')->get_point_x()) + move_x, (*this).upward_mechanism.four_bar_get_point('a')->get_point_y() + move_y);
            current_robot_arm.upward_mechanism.four_bar_set_point('b', ((*this).upward_mechanism.four_bar_get_point('b')->get_point_x()) + move_x, (*this).upward_mechanism.four_bar_get_point('b')->get_point_y() + move_y);
            current_robot_arm.upward_mechanism.four_bar_set_point('c', ((*this).upward_mechanism.four_bar_get_point('c')->get_point_x()) + move_x, (*this).upward_mechanism.four_bar_get_point('c')->get_point_y() + move_y);
            current_robot_arm.upward_mechanism.four_bar_set_point('d', ((*this).upward_mechanism.four_bar_get_point('d')->get_point_x()) + move_x, (*this).upward_mechanism.four_bar_get_point('d')->get_point_y() + move_y);
            current_robot_arm.linkage_triangle.triangle_set_point('a', ((*this).linkage_triangle.triangle_get_point('a')->get_point_x()) + move_x, (*this).linkage_triangle.triangle_get_point('a')->get_point_y() + move_y);
            current_robot_arm.linkage_triangle.triangle_set_point('b', ((*this).linkage_triangle.triangle_get_point('b')->get_point_x()) + move_x, (*this).linkage_triangle.triangle_get_point('b')->get_point_y() + move_y);
            current_robot_arm.linkage_triangle.triangle_set_point('c', ((*this).linkage_triangle.triangle_get_point('c')->get_point_x()) + move_x, (*this).linkage_triangle.triangle_get_point('c')->get_point_y() + move_y);
            current_robot_arm.driven_mechanism.four_bar_set_point('a', ((*this).driven_mechanism.four_bar_get_point('a')->get_point_x()) + move_x, (*this).driven_mechanism.four_bar_get_point('a')->get_point_y() + move_y);
            current_robot_arm.driven_mechanism.four_bar_set_point('b', ((*this).driven_mechanism.four_bar_get_point('b')->get_point_x()) + move_x, (*this).driven_mechanism.four_bar_get_point('b')->get_point_y() + move_y);
            current_robot_arm.driven_mechanism.four_bar_set_point('c', ((*this).driven_mechanism.four_bar_get_point('c')->get_point_x()) + move_x, (*this).driven_mechanism.four_bar_get_point('c')->get_point_y() + move_y);
            current_robot_arm.driven_mechanism.four_bar_set_point('d', ((*this).driven_mechanism.four_bar_get_point('d')->get_point_x()) + move_x, (*this).driven_mechanism.four_bar_get_point('d')->get_point_y() + move_y);
            current_robot_arm.end_grip.grip_set_point('a', ((*this).end_grip.triangle_get_point('a')->get_point_x()) + move_x, (*this).end_grip.triangle_get_point('a')->get_point_y() + move_y);
            current_robot_arm.end_grip.grip_set_point('b', ((*this).end_grip.triangle_get_point('b')->get_point_x()) + move_x, (*this).end_grip.triangle_get_point('b')->get_point_y() + move_y);
            current_robot_arm.end_grip.grip_set_point('c', ((*this).end_grip.triangle_get_point('c')->get_point_x()) + move_x, (*this).end_grip.triangle_get_point('c')->get_point_y() + move_y);
            current_robot_arm.end_grip.grip_set_point('d', ((*this).end_grip.grip_collision_point()->get_point_x()) + move_x, (*this).end_grip.grip_collision_point()->get_point_y() + move_y);
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
        }
    }
    float get_origin_position(const char which_size) {
        if (which_size == 'x') return (*this).origin.get_point_x();
        if (which_size == 'y') return (*this).origin.get_point_y();
        else return 0;
    }
    const float robot_arm_get_searched_angle(const char& current_angle) const {
        if (current_angle == '1') {
            return theta_M1_searched;
        }
        else if (current_angle == '2') {
            return theta_M2_searched;
        }
        else {
            std::cout << "There is no such angle for current robot arm" << std::endl;
            return 0;
        }
    }
    const float robot_arm_get_angle(const char& current_angle) const {
        if (current_angle == '1') {
            return theta_M1;
        }
        else if (current_angle == '2') {
            return theta_M2;
        }
        else {
            std::cout << "There is no such angle for current robot arm" << std::endl;
            return 0;
        }
    }
    void search_point_robot_arm(const Robot_arm& robot_arm_to_find_point, const float& x_to_search, const float& y_to_search, const float& search_point_tolerance) {
        (*this).mov_robot_arm_by_servos(0.0f,0.0f,false);
        robot_arm_to_find_point.theta_M1_searched = 0;
        robot_arm_to_find_point.theta_M2_searched = 0;
        Robot_arm temp_robot_arm = robot_arm_to_find_point;
        bool test_max_M1 = 1, test_max_M2 = 1;
        float dbetweenvec = 0, dbetweenvec_achieved = 999, pace = 20 * 3.14159265 / 180.0, P10_achieved_x = x_to_search, P10_achieved_y = y_to_search;
        float temp_theta_M1, temp_theta_M2, theta_M1_min_i = robot_arm_to_find_point.theta_M1_min, theta_M1_max_i = robot_arm_to_find_point.theta_M1_max, theta_M2_min_i = robot_arm_to_find_point.theta_M2_min, theta_M2_max_i = robot_arm_to_find_point.theta_M2_max, temp_theta_M1_degrees, temp_theta_M2_degrees;
        int max_search_loops = 3, out_range_it = 0;
        float x_to_search_rot_and_mirror = x_to_search, y_to_search_rot_and_mirror = y_to_search;
        if (temp_robot_arm.xz_angle != 0.0) {
            temp_robot_arm.angle_robot_arm(-xz_angle,false);
            x_to_search_rot_and_mirror = ((x_to_search- (*this).backward_mechanism.four_bar_get_point('a')->get_point_x()) * cos(-xz_angle) + (y_to_search- (*this).backward_mechanism.four_bar_get_point('a')->get_point_y()) * sin(-xz_angle)) + (*this).backward_mechanism.four_bar_get_point('a')->get_point_x();
            y_to_search_rot_and_mirror = ((x_to_search- (*this).backward_mechanism.four_bar_get_point('a')->get_point_x()) * (-sin(-xz_angle)) + (y_to_search- (*this).backward_mechanism.four_bar_get_point('a')->get_point_y()) * cos(-xz_angle)) + (*this).backward_mechanism.four_bar_get_point('a')->get_point_y();
        }
        if (temp_robot_arm.is_mirrorred) {
            temp_robot_arm.mirror_robot_arm();
            x_to_search_rot_and_mirror = 2 * temp_robot_arm.backward_mechanism.four_bar_get_point('a')->get_point_x() - x_to_search_rot_and_mirror;
        }
        dbetweenvec = sqrt(((x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x()) * (x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x())) + ((y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y()) * (y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y())));
        do {
            pace = pace / 10;
            test_max_M1 = 1;
            test_max_M2 = 1;
            temp_theta_M1 = theta_M1_min_i;
            temp_theta_M2 = theta_M2_min_i;
            for (temp_theta_M1 = theta_M1_min_i; temp_theta_M1 <= theta_M1_max_i; ) {
                test_max_M1 = 0;
                if (temp_theta_M1 != theta_M1_max_i) test_max_M1 = 1;
                for (temp_theta_M2 = theta_M2_min_i; temp_theta_M2 <= theta_M2_max_i; ) {
                    test_max_M2 = 0;
                    if (temp_theta_M2 != theta_M2_max_i) test_max_M2 = 1;
                    temp_robot_arm.set_robot_arm_points(temp_theta_M1, temp_theta_M2, true);
                    dbetweenvec = sqrt(((x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x()) * (x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x())) + ((y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y()) * (y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y())));
                    if (dbetweenvec < dbetweenvec_achieved) {
                        dbetweenvec_achieved = dbetweenvec;
                        P10_achieved_x = temp_robot_arm.end_grip.grip_collision_point()->get_point_x();
                        P10_achieved_y = temp_robot_arm.end_grip.grip_collision_point()->get_point_y();
                        robot_arm_to_find_point.theta_M1_searched = temp_theta_M1;
                        robot_arm_to_find_point.theta_M2_searched = temp_theta_M2;
                    }
                    temp_theta_M2 = temp_theta_M2 + pace;
                    if (test_max_M2 && temp_theta_M2 > theta_M2_max_i) { temp_theta_M2 = theta_M2_max_i; }
                }
                temp_theta_M1 = temp_theta_M1 + pace;
                if (test_max_M1 && temp_theta_M1 > theta_M1_max_i) { temp_theta_M1 = theta_M1_max_i; }
            }
            theta_M1_min_i = theta_M1_searched - pace * 10;
            theta_M1_max_i = theta_M1_searched + pace * 10;
            theta_M2_min_i = theta_M2_searched - pace * 10;
            theta_M2_max_i = theta_M2_searched + pace * 10;
            if (theta_M1_min_i < robot_arm_to_find_point.theta_M1_min) {
                theta_M1_min_i = robot_arm_to_find_point.theta_M1_min;
            }
            if (theta_M1_max_i > robot_arm_to_find_point.theta_M1_max) {
                theta_M1_max_i = robot_arm_to_find_point.theta_M1_max;
            }
            if (theta_M2_min_i < robot_arm_to_find_point.theta_M2_min) {
                theta_M2_min_i = robot_arm_to_find_point.theta_M2_min;
            }
            if (theta_M2_max_i > robot_arm_to_find_point.theta_M2_max) {
                theta_M2_max_i = robot_arm_to_find_point.theta_M2_max;
            }
            out_range_it = out_range_it + 1;
            if (out_range_it > max_search_loops) {
                std::cout << "Point is out of range for the considered precision" << std::endl;
                if (dbetweenvec_achieved > search_point_tolerance) {
                    robot_arm_to_find_point.theta_M1_searched = 0;
                    robot_arm_to_find_point.theta_M2_searched = 0;
                }
                break;
            }
        } while (dbetweenvec > search_point_tolerance);
        if (dbetweenvec_achieved > search_point_tolerance) {
            robot_arm_to_find_point.theta_M1_searched = 0;
            robot_arm_to_find_point.theta_M2_searched = 0;
        }
    }
    //The function below is not working properly yet (compare to function above to fix it)
    void search_servo_point_robot_arm(const Robot_arm& robot_arm_to_find_point, const float& x_to_search, const float& y_to_search, const float& search_point_tolerance, const float& calibrated_servo_pace) {
        int count_m1 = 0, count_m2 = 0;
        robot_arm_to_find_point.theta_M1_searched = 0;
        robot_arm_to_find_point.theta_M2_searched = 0;
        Robot_arm temp_robot_arm = robot_arm_to_find_point;
        bool test_max_M1 = 1, test_max_M2 = 1;
        float dbetweenvec = 0, dbetweenvec_achieved = 999, pace = -calibrated_servo_pace, P10_achieved_x = x_to_search, P10_achieved_y = y_to_search;
        float temp_theta_M1, temp_theta_M2, theta_M1_min_i = robot_arm_to_find_point.theta_M1_min, theta_M1_max_i = robot_arm_to_find_point.theta_M1_max, theta_M2_min_i = robot_arm_to_find_point.theta_M2_min, theta_M2_max_i = robot_arm_to_find_point.theta_M2_max, temp_theta_M1_degrees, temp_theta_M2_degrees;
        int max_search_loops = 1, out_range_it = 0;
        float x_to_search_rot_and_mirror = x_to_search, y_to_search_rot_and_mirror = y_to_search;
        if (temp_robot_arm.xz_angle != 0.0) {
            temp_robot_arm.angle_robot_arm(-xz_angle,false);
            x_to_search_rot_and_mirror = x_to_search * cos(-xz_angle) + y_to_search * sin(-xz_angle);
            y_to_search_rot_and_mirror = x_to_search * (-sin(-xz_angle)) + y_to_search * cos(-xz_angle);
        }
        if (temp_robot_arm.is_mirrorred) {
            temp_robot_arm.mirror_robot_arm();
            x_to_search_rot_and_mirror = 2 * temp_robot_arm.backward_mechanism.four_bar_get_point('a')->get_point_x() - x_to_search_rot_and_mirror;
        }
        dbetweenvec = sqrt(((x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x()) * (x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x())) + ((y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y()) * (y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y())));
        do {
            pace = -calibrated_servo_pace;
            test_max_M1 = 1;
            test_max_M2 = 1;
            temp_theta_M1 = theta_M1_min_i;
            temp_theta_M2 = theta_M2_min_i;
            for (temp_theta_M1 = theta_M1_max_i; temp_theta_M1 >= theta_M1_min_i;) {
                count_m1++;
                test_max_M1 = 0;
                if (temp_theta_M1 != theta_M1_max_i) test_max_M1 = 1;
                for (temp_theta_M2 = theta_M2_max_i; temp_theta_M2 >= theta_M2_min_i;) {
                    count_m2++;
                    test_max_M2 = 0;
                    if (temp_theta_M2 != theta_M2_max_i) test_max_M2 = 1;
                    temp_robot_arm.set_robot_arm_points(temp_theta_M1, temp_theta_M2, true);
                    dbetweenvec = sqrt(((x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x()) * (x_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_x())) + ((y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y()) * (y_to_search_rot_and_mirror - temp_robot_arm.end_grip.grip_collision_point()->get_point_y())));
                    if (dbetweenvec < dbetweenvec_achieved) {
                        dbetweenvec_achieved = dbetweenvec;
                        P10_achieved_x = temp_robot_arm.end_grip.grip_collision_point()->get_point_x();
                        P10_achieved_y = temp_robot_arm.end_grip.grip_collision_point()->get_point_y();
                        robot_arm_to_find_point.theta_M1_searched = temp_theta_M1;
                        robot_arm_to_find_point.theta_M2_searched = temp_theta_M2;
                    }
                    temp_theta_M2 = temp_theta_M2 + pace;
                    if (test_max_M2 && temp_theta_M2 > theta_M2_max_i) { temp_theta_M2 = theta_M2_max_i; }
                    if (count_m2 >= 16) {
                        count_m2 = 0;
                        break;
                    }
                }
                temp_theta_M1 = temp_theta_M1 + pace;
                if (test_max_M1 && temp_theta_M1 > theta_M1_max_i) { temp_theta_M1 = theta_M1_max_i; }
                if (count_m1 >= 16) {
                    count_m1 = 0;
                    break;
                }
            }
            theta_M1_min_i = theta_M1_searched - pace * 10;
            theta_M1_max_i = theta_M1_searched + pace * 10;
            theta_M2_min_i = theta_M2_searched - pace * 10;
            theta_M2_max_i = theta_M2_searched + pace * 10;
            if (theta_M1_min_i < robot_arm_to_find_point.theta_M1_min) {
                theta_M1_min_i = robot_arm_to_find_point.theta_M1_min;
            }
            if (theta_M1_max_i > robot_arm_to_find_point.theta_M1_max) {
                theta_M1_max_i = robot_arm_to_find_point.theta_M1_max;
            }
            if (theta_M2_min_i < robot_arm_to_find_point.theta_M2_min) {
                theta_M2_min_i = robot_arm_to_find_point.theta_M2_min;
            }
            if (theta_M2_max_i > robot_arm_to_find_point.theta_M2_max) {
                theta_M2_max_i = robot_arm_to_find_point.theta_M2_max;
            }
            out_range_it = out_range_it + 1;
            if (out_range_it > max_search_loops) {
                std::cout << "Point is out of range for the considered precision" << std::endl;
                if (dbetweenvec_achieved > search_point_tolerance) {
                    robot_arm_to_find_point.theta_M1_searched = 0;
                    robot_arm_to_find_point.theta_M2_searched = 0;
                }
                break;
            }
        } while (dbetweenvec > search_point_tolerance);
        if (dbetweenvec_achieved > 5) {
            robot_arm_to_find_point.theta_M1_searched = 0;
            robot_arm_to_find_point.theta_M2_searched = 0;
        }

    }
    void change_robot_mirror_status() {
        if ((*this).is_allowed_to_move()) {
            (*this).is_mirrorred = !(*this).is_mirrorred;
            if ((*this).end_grip.holded_box) {
                //Move the box when it is holded and the robot is mirrorred
                float temp_x = 0.0f, temp_y = 0.0f;
                if ((*this).xz_angle == 0) {
                    (*this).mirror_robot_arm();
                    temp_x = (2 * ((*this).backward_mechanism.four_bar_get_point('a')->get_point_x() - ((*this).end_grip.holded_box->box_get_point('a')->get_point_x()) - (*this).end_grip.holded_box->box_Get_Size('l') / 2));
                    (*this).end_grip.holded_box->mov_box_coordinates(*(*this).end_grip.holded_box, temp_x, temp_y);
                    (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
                }
                else if ((*this).xz_angle != -(*this).end_grip.holded_box->box_get_angle()) {
                    float temp_Ax = (*this).end_grip.holded_box->box_get_point('a')->get_point_x(), temp_Ay = (*this).end_grip.holded_box->box_get_point('a')->get_point_y();
                    float temp_Bx = (*this).end_grip.holded_box->box_get_point('b')->get_point_x(), temp_By = (*this).end_grip.holded_box->box_get_point('b')->get_point_y();
                    float temp_Cx = (*this).end_grip.holded_box->box_get_point('c')->get_point_x(), temp_Cy = (*this).end_grip.holded_box->box_get_point('c')->get_point_y();
                    float temp_Dx = (*this).end_grip.holded_box->box_get_point('d')->get_point_x(), temp_Dy = (*this).end_grip.holded_box->box_get_point('d')->get_point_y();
                    float temp_x = (*this).backward_mechanism.four_bar_get_point('a')->get_point_x(), temp_y = (*this).backward_mechanism.four_bar_get_point('a')->get_point_y();
                    (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
                    float thetaAB = 0.0f;
                    float al = tan((3.14159265359 / 2) - (*this).xz_angle), bl = temp_y - al * temp_x, ap = tan(-(*this).xz_angle), bp = temp_Ay - ap * temp_Ax, Xmp = (bp - bl) / (al - ap), Ymp = al * Xmp + bl, Axr = 2 * Xmp - temp_Ax, Ayr = 2 * Ymp - temp_Ay;
                    float bpb = temp_By - ap * temp_Bx, Xmpb = (bpb - bl) / (al - ap), Ympb = al * Xmpb + bl, Bxr = 2 * Xmpb - temp_Bx, Byr = 2 * Ympb - temp_By;
                    float bpc = temp_Cy - ap * temp_Cx, Xmpc = (bpc - bl) / (al - ap), Ympc = al * Xmpc + bl, Cxr = 2 * Xmpc - temp_Cx, Cyr = 2 * Ympc - temp_Cy;
                    float bpd = temp_Dy - ap * temp_Dx, Xmpd = (bpd - bl) / (al - ap), Ympd = al * Xmpd + bl, Dxr = 2 * Xmpd - temp_Dx, Dyr = 2 * Ympd - temp_Dy;
                    if ((*this).is_mirrorred) {
                        thetaAB = atan((Cyr - Ayr) / (Cxr - Axr));
                    }
                    else {
                        thetaAB = atan((Byr - Ayr) / (Bxr - Axr));
                    }
                    (*this).end_grip.holded_box->box_set_point('a', Axr, Ayr);
                    (*this).end_grip.holded_box->box_set_point('b', Bxr, Byr);
                    (*this).end_grip.holded_box->box_set_point('c', Cxr, Cyr);
                    (*this).end_grip.holded_box->box_set_point('d', Dxr, Dyr);
                    (*this).end_grip.holded_box->box_set_angle(thetaAB);
                    (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
                }
                else {
                    if ((*this).is_mirrorred) {
                        (*this).end_grip.holded_box->box_set_point('a', (((*this).end_grip.holded_box->box_get_point('a')->get_point_x())) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('a')->get_point_y()) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('b', (((*this).end_grip.holded_box->box_get_point('b')->get_point_x())) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('b')->get_point_y()) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('c', (((*this).end_grip.holded_box->box_get_point('c')->get_point_x())) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('c')->get_point_y()) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('d', (((*this).end_grip.holded_box->box_get_point('d')->get_point_x())) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('d')->get_point_y()) + (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
                    }
                    else {
                        (*this).end_grip.holded_box->box_set_point('a', (((*this).end_grip.holded_box->box_get_point('a')->get_point_x())) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('a')->get_point_y()) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('b', (((*this).end_grip.holded_box->box_get_point('b')->get_point_x())) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('b')->get_point_y()) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('c', (((*this).end_grip.holded_box->box_get_point('c')->get_point_x())) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('c')->get_point_y()) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).end_grip.holded_box->box_set_point('d', (((*this).end_grip.holded_box->box_get_point('d')->get_point_x())) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * cos(-(*this).xz_angle)), ((*this).end_grip.holded_box->box_get_point('d')->get_point_y()) - (((*this).end_grip.holded_box->box_Get_Size('l') / 4) * sin(-(*this).xz_angle)));
                        (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
                    }
                }
            }
            else {
                (*this).mov_robot_arm_by_servos(0.0f, 0.0f, false);
            }

        }
        else {
            std::cout << "Cannot turn around! Object is not allowed to move." << std::endl;
        }  
    }
    void set_robot_angle(const float& phia, const float& step_angle, const float& increment_height) {
        if ((*this).end_grip.is_allowed_to_move()) {
            float cube_init_x = 0.0f, cube_init_y = 0.0f;       
            if ((*this).end_grip.holded_box) {
                cube_init_x = (*this).end_grip.holded_box->box_get_point('a')->get_point_x();
                cube_init_y = (*this).end_grip.holded_box->box_get_point('a')->get_point_y();
            }        
            (*this).angle_robot_arm(-xz_angle,false);
            if ((*this).is_mirrorred) (*this).mirror_robot_arm();
            (*this).xz_angle = phia;                                                                      
            if ((*this).is_mirrorred) (*this).mirror_robot_arm();                                         
            (*this).angle_robot_arm(xz_angle,true);   
            (*this).set_robot_origin(((*this).Initial_robot_x - ((*this).Initial_robot_y- increment_height) * sin(-phia)) * 2.38f, ((((*this).Initial_robot_y- increment_height)*cos(-phia)) + 0.237f + increment_height) * 2.38f);
            (*this).mov_robot_arm_by_servos(0.0f,0.0f,false);
            if ((*this).end_grip.holded_box) {
                (*this).end_grip.holded_box->box_set_robot_actual_angle((*this).xz_angle);
                float r = sqrt(((cube_init_x - (((*this).Initial_robot_x ) * 2.38)) * (cube_init_x - (((*this).Initial_robot_x) * 2.38))) + ((cube_init_y- increment_height * 2.38) * (cube_init_y - increment_height * 2.38)));
                float theta_around = atan2((cube_init_y - increment_height * 2.38), (cube_init_x - (((*this).Initial_robot_x) * 2.38))) + step_angle;
                (*this).end_grip.holded_box->box_set_point('a', (((*this).Initial_robot_x) * 2.38) + (r * cos(theta_around)), increment_height * 2.38 + r * sin(theta_around));
                (*this).end_grip.holded_box->box_set_point('b', (*this).end_grip.holded_box->box_get_point('a')->get_point_x() + (*this).end_grip.holded_box->box_Get_Size('l') * cos((*this).end_grip.holded_box->box_get_angle()), (*this).end_grip.holded_box->box_get_point('a')->get_point_y() + (*this).end_grip.holded_box->box_Get_Size('l') * sin((*this).end_grip.holded_box->box_get_angle()));
                (*this).end_grip.holded_box->box_set_point('c', (*this).end_grip.holded_box->box_get_point('a')->get_point_x() + (*this).end_grip.holded_box->box_Get_Size('h') * cos((*this).end_grip.holded_box->box_get_angle()+ 3.1415926536/2), (*this).end_grip.holded_box->box_get_point('a')->get_point_y() + (*this).end_grip.holded_box->box_Get_Size('h') * sin((*this).end_grip.holded_box->box_get_angle() + 3.1415926536/2));
                (*this).end_grip.holded_box->box_set_point('d', (*this).end_grip.holded_box->box_get_point('c')->get_point_x() + (*this).end_grip.holded_box->box_Get_Size('l') * cos((*this).end_grip.holded_box->box_get_angle()), (*this).end_grip.holded_box->box_get_point('c')->get_point_y() + (*this).end_grip.holded_box->box_Get_Size('l') * sin((*this).end_grip.holded_box->box_get_angle()));
            }
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
        }
    }
    void set_robot_init_position(const float& initial_x_position, const float& initial_y_position) {
        (*this).Initial_robot_x = initial_x_position;
        (*this).Initial_robot_y = initial_y_position;
    }
    void set_robot_origin(const float& x_coordinate, const float& y_coordinate) {
        if ((*this).end_grip.is_allowed_to_move()) {
            origin.set_point(x_coordinate, y_coordinate);
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
        }
    }
    float get_initial_robot_position(char position) {
        if (position == 'x') return Initial_robot_x;
        if (position == 'y') return Initial_robot_y;
        return 0;
    }
    void print_arm_points() const {
        std::cout << "The points are printed in the order shown in the image: " << std::endl;
        std::cout << "Point 1 x: " << backward_mechanism.four_bar_get_point('a')->get_point_x() << ", Point 1 y: " << backward_mechanism.four_bar_get_point('a')->get_point_y() << std::endl;
        std::cout << "Point 2 x: " << backward_mechanism.four_bar_get_point('b')->get_point_x() << ", Point 2 y: " << backward_mechanism.four_bar_get_point('b')->get_point_y() << std::endl;
        std::cout << "Point 3 x: " << backward_mechanism.four_bar_get_point('d')->get_point_x() << ", Point 3 y: " << backward_mechanism.four_bar_get_point('d')->get_point_y() << std::endl;
        std::cout << "Point 4 x: " << backward_mechanism.four_bar_get_point('c')->get_point_x() << ", Point 4 y: " << backward_mechanism.four_bar_get_point('c')->get_point_y() << std::endl;
        std::cout << "Point 5 x: " << upward_mechanism.four_bar_get_point('b')->get_point_x() << ", Point 5 y: " << upward_mechanism.four_bar_get_point('b')->get_point_y() << std::endl;
        std::cout << "Point 6 x: " << upward_mechanism.four_bar_get_point('c')->get_point_x() << ", Point 6 y: " << upward_mechanism.four_bar_get_point('c')->get_point_y() << std::endl;
        std::cout << "Point 7 x: " << driven_mechanism.four_bar_get_point('b')->get_point_x() << ", Point 7 y: " << driven_mechanism.four_bar_get_point('b')->get_point_y() << std::endl;
        std::cout << "Point 8 x: " << driven_mechanism.four_bar_get_point('d')->get_point_x() << ", Point 8 y: " << driven_mechanism.four_bar_get_point('d')->get_point_y() << std::endl;
        std::cout << "Point 9 x: " << driven_mechanism.four_bar_get_point('c')->get_point_x() << ", Point 9 y: " << driven_mechanism.four_bar_get_point('c')->get_point_y() << std::endl;
        std::cout << "Point 10 x: " << end_grip.grip_collision_point()->get_point_x() << ", Point 10 y: " << end_grip.grip_collision_point()->get_point_y() << std::endl;
    }
    void update_arm_points(float* points_to_print) const {
        points_to_print[0] = backward_mechanism.four_bar_get_point('a')->get_point_x();
        points_to_print[1] = backward_mechanism.four_bar_get_point('a')->get_point_y();
        points_to_print[2] = backward_mechanism.four_bar_get_point('b')->get_point_x();
        points_to_print[3] = backward_mechanism.four_bar_get_point('b')->get_point_y();
        points_to_print[4] = backward_mechanism.four_bar_get_point('d')->get_point_x();
        points_to_print[5] = backward_mechanism.four_bar_get_point('d')->get_point_y();
        points_to_print[6] = backward_mechanism.four_bar_get_point('c')->get_point_x();
        points_to_print[7] = backward_mechanism.four_bar_get_point('c')->get_point_y();
        points_to_print[8] = upward_mechanism.four_bar_get_point('b')->get_point_x();
        points_to_print[9] = upward_mechanism.four_bar_get_point('b')->get_point_y();
        points_to_print[10] = upward_mechanism.four_bar_get_point('c')->get_point_x();
        points_to_print[11] = upward_mechanism.four_bar_get_point('c')->get_point_y();
        points_to_print[12] = driven_mechanism.four_bar_get_point('b')->get_point_x();
        points_to_print[13] = driven_mechanism.four_bar_get_point('b')->get_point_y();
        points_to_print[14] = driven_mechanism.four_bar_get_point('d')->get_point_x();
        points_to_print[15] = driven_mechanism.four_bar_get_point('d')->get_point_y();
        points_to_print[16] = driven_mechanism.four_bar_get_point('c')->get_point_x();
        points_to_print[17] = driven_mechanism.four_bar_get_point('c')->get_point_y();
        points_to_print[18] = end_grip.grip_collision_point()->get_point_x();
        points_to_print[19] = end_grip.grip_collision_point()->get_point_y();
    }
    void reset_robot_arm_searched_angle() {
        theta_M1_searched = 0.0f;
        theta_M2_searched = 0.0f;
    }
};
class Car : public Box {
private:
    float Vx, Vy;
public:
    Car(const Car& to_copy) : Box((static_cast<Box>(to_copy)).box_get_point('a')->get_point_x(), (static_cast<Box>(to_copy)).box_get_point('b')->get_point_x() - (static_cast<Box>(to_copy)).box_get_point('a')->get_point_x(), (static_cast<Box>(to_copy)).box_get_point('c')->get_point_y()) {
        to_copy.Box_Get_Counting(0);
        to_copy.Car_Get_Counting(1);
        (*this).Vx = to_copy.Vx;
        (*this).Vy = to_copy.Vy;
    }
    Car(const float inicial_Vx, const float inicial_Vy, const float inicial_x, const float lenght, const float height) : Vx(inicial_Vx), Vy(inicial_Vy), Box(inicial_x, lenght, height) {
        (*this).Box_Get_Counting(0);
        (*this).Car_Get_Counting(1);
    }
    Car(const float inicial_Vx, const float inicial_Vy, const float inicial_x, const float cube_side) : Vx(inicial_Vx), Vy(inicial_Vy), Box(inicial_x, cube_side) {
        (*this).Box_Get_Counting(0);
        (*this).Car_Get_Counting(1);
    }
    ~Car() {
        (*this).Box_Get_Counting(1);
        (*this).Car_Get_Counting(0);
#ifdef _DEBUG 
        std::cout << "Car was destructed" << std::endl;
#endif      
    }

    static int Car_Get_Counting(int count) {
        static int number_of_cars = 0;
        if (count == 1) number_of_cars++;
        if (count == 0) number_of_cars--;
        else return number_of_cars;
    }
};
class Robot_Car : public Car, public Robot_arm {
private:

public:
    Robot_Car(const float inicial_x) : Car(0, 0, inicial_x, 10, 5), Robot_arm() {
        (*this).Robot_Get_Counting(0);
        (*this).Car_Get_Counting(0);
        (*this).Robot_Car_Get_Counting(1);
        (*this).set_robot_origin(inicial_x + (10 / 2), 5);
        (*this).set_robot_arm_points((*this).robot_arm_get_angle('1'), (*this).robot_arm_get_angle('2'), false);
    }
    Robot_Car(const float inicial_Vx, const float inicial_Vy, const float inicial_x, const float lenght, const float height) : Car(inicial_Vx, inicial_Vy, inicial_x, lenght, height), Robot_arm() {
        (*this).Robot_Get_Counting(0);
        (*this).Car_Get_Counting(0);
        (*this).Robot_Car_Get_Counting(1);
        (*this).set_robot_origin(inicial_x + (lenght / 2), height);
        (*this).set_robot_arm_points((*this).robot_arm_get_angle('1'), (*this).robot_arm_get_angle('2'), false);
    }
    Robot_Car(float Initial_robot_xa, float Initial_robot_ya, const float inicial_Vx, const float inicial_Vy, const float inicial_x, const float lenght, const float height, const bool& mirrorred, const Point& origina, const float& xz_anglea, const float& theta_M1_maxa, const float& theta_M1_mina, const float& theta_M2_maxa, const float& theta_M2_mina, const float& lea, const float& lca, const float& laa, const float& lfa, const float& lia, const float& lda, const float& lha, const float& lga, const float& lba, const float& lja, const float& lka, const float& theta1a, const float& theta37a, const float& theta810a, const float& theta_M1a, const float& theta_M2a, const float& forward_max_contacta, const float& backward_max_contacta, const float& Maximum_angle_long_arma) : Car(inicial_Vx, inicial_Vy, inicial_x, lenght, height), Robot_arm(Initial_robot_xa, Initial_robot_ya, mirrorred, origina, xz_anglea, theta_M1_maxa, theta_M1_mina, theta_M2_maxa, theta_M2_mina, lea, lca, laa, lfa, lia, lda, lha, lga, lba, lja, lka, theta1a, theta37a, theta810a, theta_M1a, theta_M2a, forward_max_contacta, backward_max_contacta, Maximum_angle_long_arma) {
        (*this).Robot_Get_Counting(0);
        (*this).Car_Get_Counting(0);
        (*this).Robot_Car_Get_Counting(1);
        (*this).set_robot_origin(inicial_x + (lenght / 2), height);
        (*this).set_robot_arm_points((*this).robot_arm_get_angle('1'), (*this).robot_arm_get_angle('2'), false);
    }
    ~Robot_Car() {
        (*this).Robot_Get_Counting(1);
        (*this).Box_Get_Counting(1);
        (*this).Car_Get_Counting(1);
        (*this).Robot_Car_Get_Counting(0);
#ifdef _DEBUG 
        std::cout << "Robot car was destructed" << std::endl;
#endif 
    }

    void mov_robot_car_coordinates(Robot_Car& current_robot_arm, float move_x, float move_y) {
        if ((*this).is_allowed_to_move()) {
            current_robot_arm.mov_box_coordinates(current_robot_arm, move_x, move_y);
            current_robot_arm.mov_robot_arm_coordinates(current_robot_arm, move_x, move_y);
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
        }
    }
    void rotate_robot_car(float phi) {
        if ((*this).is_allowed_to_move()) {
            (*this).set_robot_origin(((*this).box_get_point('c')->get_point_x() + (*this).box_get_point('d')->get_point_x()) / 2, ((*this).box_get_point('c')->get_point_y() + (*this).box_get_point('d')->get_point_y()) / 2);
            //(*this).set_robot_angle(phi);
        }
        else {
            std::cout << "The robot is not allowed to move because it is stuck with cube holded by more than one robot !" << std::endl;
        }
    }
    static int Robot_Car_Get_Counting(int count) {
        static int number_of_robot_cars = 0;
        if (count == 1) number_of_robot_cars++;
        if (count == 0) number_of_robot_cars--;
        else return number_of_robot_cars;
    }
};

void Scene() {
    void Init_scene(); // Files to open and decide what step
    void SelectComponents(); //Select cars, select robot arms (Logic for mirror and angle and attach), select cubes (And attachments) 
    void BuildPath(); //Choose point and check possibility, create deleting chosen point, creat attachment, select points, select what happens simultaneously
    void CheckCollision(); //Save last P10 and angles
    void PrintAnglesToFile(); //With robot number
    void ShowMovementOnScreen();
};

void Reading_and_writing_bula_files() {
    std::fstream my_file;
    my_file.open("start.bula", std::ios::out); //app to append
    if (my_file.is_open()) {
        my_file << " First bula file was created " << std::endl;
    }
    my_file.close();
    my_file.open("start.bula", std::ios::in); //myFile.open(“saldinaFile.txt”, ios::out | ios::in); for reading and writing
    std::string line;
    if (my_file.is_open()) {
        while (std::getline(my_file, line)) {
            std::cout << line << std::endl;
        }
    }
    my_file.close();
}