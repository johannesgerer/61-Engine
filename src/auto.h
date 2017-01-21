#ifndef _AUTO_H
#define _AUTO_H
class CTire:public CVector3{

public:
	CTire(){x=2;y=-1;z=1;};
	CTire(float ix, float iy, float iz){x=ix;y=iy;z=iz;};
	CTire(CVector3 v){x=v.x;y=v.y;z=v.z;};

	float m_gewicht;
	//void RenderVektor(CVector3 start,float size);
	
	
};
class CAuto:public CZiel{

public:
	
	CAuto(){};

	CVector3	GetAcceleration(CVector3 v);
	float	GetMaxVelocity();

	void	SetPostition	(CVector3 position)		{m_p=position;};
	void	SetVelocity		(CVector3 velocity)		{m_v=velocity;};
	void	SetAcceleration	(CVector3 acceleration)	{m_a=acceleration;};

	bool Movement();

	void lenken(float winkel) {m_winkel_lenkung=winkel;};

	void gas_geben();

	void update();

	void render();

	void textout();

protected:

	float m_masse;	//(kg)
	float m_gas;	//(N)
	float m_bremse;	//(N)
	float m_radeinschlag; //(DEGREE)
	float m_radstand;

	float m_lagerung;

	float reibung;
	float luftwiderstand;
	float speed;
		
	float lenkspeed;

	float t;	//(s)
	float t_null;	//(s)
	float i;

	CCamera* Camera;

	CVector3 m_p;
	CVector3 m_p_null;
	
	CVector3 m_v;
	CVector3 m_v_null;

	CVector3 m_f;
	CVector3 m_a;
	CVector3 m_richtung;

	bool gas;
	bool bremse;
	bool kraft;
	bool lenkung;

	CTire tire[4];

	float m_winkel_lenkung;

};
class CF40:public CAuto{

public:
	CF40();
		
};


#endif